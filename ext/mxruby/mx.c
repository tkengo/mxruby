/*
 * Copyright (c) 2016 Kengo Tateishi (@tkengo)
 * Licensed under MIT license.
 *   http://www.opensource.org/licenses/mit-license.php
 *
 * MxRuby is really inspired by numpy.
 *
 * This file is implementation of extended array functions for ruby. The MX class is defined on ruby
 * interpreter by compiling it.
 */
#include <ruby.h>
#include <cblas.h>
#include "mx.h"

/*
 * MX class object in C. This is initialized in Init_mxruby function.
 */
VALUE rb_cMx;
VALUE rb_eDataTypeError;

MX *mxx_initialize(VALUE shape, DTYPE dtype)
{
    MX *mx = MX_ALLOC(MX);
    mxx_setup(mx, dtype);
    mxx_initialize_shape(mx, shape);

    if (mx->size > 0) {
        mx->elptr = MX_ALLOC_N(char, DTYPE_SIZES[mx->dtype] * mx->size);
    }

    return mx;
}

void mxx_setup(MX *mx, DTYPE dtype)
{
    mx->dim = 0;
    mx->size = 0;
    mx->dtype = dtype;
    mx->shape = NULL;
    mx->elptr = NULL;
}

void mxx_initialize_shape(MX *mx, VALUE shape)
{
    char shape_is_array = TYPE(shape) == T_ARRAY;

    mx->size = 1;
    mx->dim = shape_is_array ? RARRAY_LEN(shape) : 1;
    mx->shape = MX_ALLOC_N(size_t, mx->dim);

    for (int i = 0; i < mx->dim; i++) {
        mx->shape[i] = FIX2INT(shape_is_array ? RARRAY_AREF(shape, i) : shape);
        mx->size *= mx->shape[i];
    }
}

void mxx_free(MX *mx)
{
    if (mx->shape != NULL) {
        MX_FREE(mx->shape);
    }
    if (mx->elptr != NULL) {
        MX_FREE(mx->elptr);
    }

    MX_FREE(mx);
}

static VALUE mx_alloc(VALUE klass)
{
    MX *mx = MX_ALLOC(MX);
    mxx_setup(mx, DTYPE_FLOAT64);

    return Data_Wrap_Struct(klass, 0, mxx_free, mx);
}

static VALUE mx_initialize(VALUE self, VALUE shape, VALUE initial_value, VALUE opt)
{
    MX *mx;
    Data_Get_Struct(self, MX, mx);

    if (!NIL_P(opt)) {
        VALUE rb_dtype = rb_hash_aref(opt, ID2SYM(rb_intern("dtype")));
        if (!NIL_P(rb_dtype)) {
            mx->dtype = mxx_dtype_from_symbol(rb_dtype);
        }
    }

    mxx_initialize_shape(mx, shape);

    if (mx->size > 0) {
        mx->elptr = MX_ALLOC_N(char, DTYPE_SIZES[mx->dtype] * mx->size);

        if (TYPE(initial_value) == T_ARRAY) {
            for (int i = 0; i < mx->size; i++) {
                VALUE v = RARRAY_AREF(initial_value, i);
                mxx_rb_to_c(mx->elptr + i * DTYPE_SIZES[mx->dtype], mx->dtype, v);
            }
        } else if (!NIL_P(initial_value)) {
            for (int i = 0; i < mx->size; i++) {
                mxx_rb_to_c(mx->elptr + i * DTYPE_SIZES[mx->dtype], mx->dtype, initial_value);
            }
        }
    }

    return self;
}

static VALUE mx_shape(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    VALUE *shape = MX_ALLOC_N(VALUE, mx->dim);
    for (int i = 0; i < mx->dim; i++) {
        shape[i] = INT2FIX(mx->shape[i]);
    }

    return rb_ary_new4(mx->dim, shape);
}

static VALUE mx_dim(VALUE self)
{
    return INT2FIX(MX_DATA_DIM(self));
}

static VALUE mx_size(VALUE self)
{
    return INT2FIX(MX_DATA_SIZE(self));
}

static VALUE mx_dtype(VALUE self)
{
    return ID2SYM(rb_intern(DTYPE_NAMES[MX_DATA_PTR(self)->dtype]));
}

static VALUE mx_dot(VALUE self, VALUE target)
{
    int N = 2;
    MX *mx = MX_DATA_PTR(self);
    void *c = MX_ALLOC_N(char, mx->shape[0] * mx->shape[1] * DTYPE_SIZES[mx->dtype]);

    cblas_sgemm(
            CblasRowMajor, CblasNoTrans, CblasNoTrans,
            N, N, N,
            1, mx->elptr, N,
            MX_DATA_ELPTR(target), N,
            0, c, N
            );

    MX_FREE(mx->elptr);
    mx->elptr = c;
    return self;
}

static VALUE mx_astype(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_new_dtype, rb_opt;
    rb_scan_args(argc, argv, "1:", &rb_new_dtype, &rb_opt);

    DTYPE new_dtype = mxx_dtype_from_symbol(rb_new_dtype);
    MX *dest = MX_ALLOC(MX);
    mxx_copy_cast(MX_DATA_PTR(self), dest, new_dtype);

    return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, dest);
}

static void mxs_to_a(VALUE *ary, size_t shape_index, size_t array_index, MX *mx)
{
    if (shape_index == mx->dim - 1) {
        for (int i = 0; i < mx->shape[shape_index]; i++) {
            void *p = (char *)mx->elptr + (i + array_index) * DTYPE_SIZES[mx->dtype];
            ary[i] = mxx_c_to_rb(p, mx->dtype);
        }
    } else {
        size_t skip = 1;
        for (int i = shape_index + 1; i < mx->dim; i++) {
            skip *= mx->shape[i];
        }

        for (int i = 0; i < mx->shape[shape_index]; i++) {
            VALUE *v = MX_ALLOC_N(VALUE, mx->shape[shape_index + 1]);
            mxs_to_a(v, shape_index + 1, skip * i + array_index, mx);
            ary[i] = rb_ary_new4(mx->shape[shape_index + 1], v);
        }
    }
}

static VALUE mx_to_a(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    VALUE *ary = MX_ALLOC_N(VALUE, mx->shape[0]);
    mxs_to_a(ary, 0, 0, mx);
    return rb_ary_new4(mx->shape[0], ary);
}

static VALUE mx_flatten(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    VALUE *el = MX_ALLOC_N(VALUE, mx->size);
    size_t dsize = DTYPE_SIZES[mx->dtype];
    for (int i = 0; i < mx->size; i++) {
        el[i] = mxx_c_to_rb((char *)mx->elptr + i * dsize, mx->dtype);
    }
    return rb_ary_new4(mx->size, el);
}

static VALUE mx_first(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    if (mx->size > 0) {
        return mxx_c_to_rb(mx->elptr, mx->dtype);
    }
    return Qnil;
}

static VALUE mx_second(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    if (mx->size > 1) {
        return mxx_c_to_rb(mx->elptr + DTYPE_SIZES[mx->dtype], mx->dtype);
    }
    return Qnil;
}

static VALUE mx_last(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    if (mx->size > 0) {
        return mxx_c_to_rb(mx->elptr + (mx->size - 1) * DTYPE_SIZES[mx->dtype], mx->dtype);
    }
    return Qnil;
}

static VALUE mx_reshape(VALUE self, VALUE shape)
{
    MX *mx = MX_DATA_PTR(self);

    MX *reshaped_mx = MX_ALLOC(MX);
    mxx_initialize_shape(reshaped_mx, shape);

    if (mx->size != reshaped_mx->size) {
        rb_raise(rb_eArgError, "Cannot do reshape operation between shape size of [ %ld ] and [ %ld ]. Total size of new array must be unchanged.", mx->size, reshaped_mx->size);
    }

    reshaped_mx->dtype = mx->dtype;
    reshaped_mx->elptr = MX_ALLOC_N(char, reshaped_mx->size * DTYPE_SIZES[reshaped_mx->dtype]);

    mxx_copy_elptr(mx, reshaped_mx);
    return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, reshaped_mx);
    return self;
}

static VALUE mx_is_empty(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    return mx->size == 0 ? Qtrue : Qfalse;
}

static VALUE mx_set(int argc, VALUE *argv, VALUE self)
{
    return self;
}

static VALUE mx_get(int argc, VALUE *argv, VALUE self)
{
    VALUE idx;
    rb_scan_args(argc, argv, "0*", &idx);
    MX *mx = MX_DATA_PTR(self);

    size_t idx_len = RARRAY_LEN(idx);
    size_t pos = 0;
    for (int i = 0; i < idx_len; i++) {
        size_t skip = 1;
        for (int j = i + 1; j < idx_len; j++) {
            skip *= mx->shape[j];
        }

        pos += FIX2INT(RARRAY_AREF(idx, i)) * skip;
    }

    return mxx_c_to_rb((char *)mx->elptr + pos * DTYPE_SIZES[mx->dtype], mx->dtype);
}

static VALUE mx_ewadd(VALUE self, VALUE other)
{
    MX *mx = MX_DATA_PTR(self);
    MX *new_mx = MX_ALLOC(MX);

    if (IS_MX(other)) {
        MX *other_mx = MX_DATA_PTR(other);
        if (!mxx_is_same_shape(mx, other_mx)) {
            rb_raise(rb_eDataTypeError, "Cannot do power operation between different shapes");
        }
        mxx_ewadd_array(mx, other_mx, new_mx);
    } else {
        DTYPE new_dtype = TYPE(other) == T_FLOAT ? DTYPE_FLOAT64 : mx->dtype;
        double v = NUM2DBL(other);
        mxx_copy_cast(mx, new_mx, new_dtype);
        mxx_ewadd_scalar(new_mx, v);
    }

    return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, new_mx);
}

static VALUE mx_ewsub(VALUE self, VALUE other)
{
    MX *mx = MX_DATA_PTR(self);
    MX *new_mx = MX_ALLOC(MX);

    if (IS_MX(other)) {
        MX *other_mx = MX_DATA_PTR(other);
        if (!mxx_is_same_shape(mx, other_mx)) {
            rb_raise(rb_eDataTypeError, "Cannot do power operation between different shapes");
        }
        mxx_ewsub_array(mx, other_mx, new_mx);
    } else {
        DTYPE new_dtype = TYPE(other) == T_FLOAT ? DTYPE_FLOAT64 : mx->dtype;
        double v = NUM2DBL(other);
        mxx_copy_cast(mx, new_mx, new_dtype);
        mxx_ewsub_scalar(new_mx, v);
    }

    return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, new_mx);
}

static VALUE mx_ewmul(VALUE self, VALUE other)
{
    MX *mx = MX_DATA_PTR(self);
    MX *new_mx = MX_ALLOC(MX);

    if (IS_MX(other)) {
        MX *other_mx = MX_DATA_PTR(other);
        if (!mxx_is_same_shape(mx, other_mx)) {
            rb_raise(rb_eDataTypeError, "Cannot do power operation between different shapes");
        }
        mxx_ewmul_array(mx, other_mx, new_mx);
    } else {
        DTYPE new_dtype = TYPE(other) == T_FLOAT ? DTYPE_FLOAT64 : mx->dtype;
        double v = NUM2DBL(other);
        mxx_copy_cast(mx, new_mx, new_dtype);
        mxx_ewmul_scalar(new_mx, v);
    }

    return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, new_mx);
}

static VALUE mx_ewpow(VALUE self, VALUE other)
{
    MX *mx = MX_DATA_PTR(self);
    MX *new_mx = MX_ALLOC(MX);

    if (IS_MX(other)) {
        MX *other_mx = MX_DATA_PTR(other);
        if (!mxx_is_same_shape(mx, other_mx)) {
            rb_raise(rb_eDataTypeError, "Cannot do power operation between different shapes");
        }

        if (DTYPE_IS_INT(other_mx->dtype)) {
            mxx_ewintpow_array(mx, other_mx, new_mx);
        } else {
            return self;
        }
    } else {
        if (TYPE(other) == T_FIXNUM) {
            mxx_copy(mx, new_mx);
            mxx_ewintpow_scalar(new_mx, FIX2INT(other));
        } else {
            mxx_copy_cast(mx, new_mx, DTYPE_FLOAT64);
            mxx_ewpow_scalar(new_mx, NUM2DBL(other));
        }
    }

    return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, new_mx);
}

static VALUE mx_sing_arange(int argc, VALUE *argv, VALUE klass)
{
    DTYPE dtype;
    double start, stop, step;
    int start_type, stop_type, step_type;
    VALUE rb_start, rb_stop, rb_step, rb_opt;
    rb_scan_args(argc, argv, "12:", &rb_start, &rb_stop, &rb_step, &rb_opt);

    if (NIL_P(rb_stop)) {
        start      = 0;
        stop       = NUM2DBL(rb_start);
        start_type = T_FIXNUM;
        stop_type  = TYPE(rb_start);
    } else {
        start      = NUM2DBL(rb_start);
        stop       = NUM2DBL(rb_stop);
        start_type = TYPE(rb_start);
        stop_type  = TYPE(rb_stop);
    }

    step      = NIL_P(rb_step) ? 1 : NUM2DBL(rb_step);
    step_type = NIL_P(rb_step) ? T_FIXNUM : TYPE(rb_step);

    if (step == 0) {
        rb_raise(rb_eArgError, "Cannot specify 0 to the step argument");
    } else if (step > 0 && start > stop) {
        rb_raise(rb_eArgError, "Confuse arguments order. start > stop.");
    } else if (step < 0 && start < stop) {
        rb_raise(rb_eArgError, "Confuse arguments order. start < stop.");
    } else if (start == stop) {
        MX *mx = MX_INIT_D(INT2FIX(0), DTYPE_INT64);
        return Data_Wrap_Struct(klass, 0, mxx_free, mx);
    }

    if (step_type == T_FIXNUM && start_type == T_FIXNUM && stop_type == T_FIXNUM) {
        dtype = DTYPE_INT64;
    } else {
        dtype = DTYPE_FLOAT64;
    }

    size_t shape = (size_t)fabs(ceil((stop - start) / step));
    MX *mx = MX_INIT_D(INT2FIX(shape), dtype);

    double current = start;
    for (int i = 0; i < mx->size; i++) {
        if (mx->dtype == DTYPE_INT64) {
            *(((int64_t *)mx->elptr) + i) = (int64_t)current;
        } else {
            *(((double *)mx->elptr) + i) = current;
        }
        current += step;
    }

    return Data_Wrap_Struct(klass, 0, mxx_free, mx);
}

static VALUE mx_sing_linspace(int argc, VALUE *argv, VALUE klass)
{
    VALUE rb_start, rb_stop, rb_shape, rb_opt;
    rb_scan_args(argc, argv, "21:", &rb_start, &rb_stop, &rb_shape, &rb_opt);

    if (NIL_P(rb_shape)) {
        rb_shape = INT2FIX(100);
    }

    if (FIX2INT(rb_shape) < 0) {
        rb_raise(rb_eArgError, "Cannot specify negative number to the shape argument");
    }

    MX *mx = MX_INIT_D(rb_shape, DTYPE_FLOAT64);
    double start = NUM2DBL(rb_start);
    double stop  = NUM2DBL(rb_stop);
    double step  = (stop - start) * (1.0 / (double)(mx->size - 1));

    double current = start;
    for (int i = 0; i < mx->size; i++) {
        *(((double *)mx->elptr) + i) = current;
        current += step;
    }

    return Data_Wrap_Struct(klass, 0, mxx_free, mx);
}

void Init_mxruby()
{
    rb_cMx = rb_define_class("MX", rb_cObject);
    rb_eDataTypeError = rb_define_class("DataTypeError", rb_eStandardError);

    rb_define_alloc_func(rb_cMx, mx_alloc);

    rb_define_protected_method(rb_cMx, "__set__", mx_initialize, 3);

    rb_define_method(rb_cMx, "shape", mx_shape, 0);
    rb_define_method(rb_cMx, "dim", mx_dim, 0);
    rb_define_method(rb_cMx, "size", mx_size, 0);
    rb_define_method(rb_cMx, "dtype", mx_dtype, 0);
    rb_define_method(rb_cMx, "dot", mx_dot, 1);
    rb_define_method(rb_cMx, "astype", mx_astype, -1);
    rb_define_method(rb_cMx, "to_a", mx_to_a, 0);
    rb_define_method(rb_cMx, "flatten", mx_flatten, 0);
    rb_define_method(rb_cMx, "first", mx_first, 0);
    rb_define_method(rb_cMx, "second", mx_second, 0);
    rb_define_method(rb_cMx, "last", mx_last, 0);
    rb_define_method(rb_cMx, "reshape", mx_reshape, 1);

    rb_define_method(rb_cMx, "empty?", mx_is_empty, 0);

    rb_define_method(rb_cMx, "[]=", mx_set, -1);
    rb_define_method(rb_cMx, "[]", mx_get, -1);
    rb_define_method(rb_cMx, "+", mx_ewadd, 1);
    rb_define_method(rb_cMx, "-", mx_ewsub, 1);
    rb_define_method(rb_cMx, "*", mx_ewmul, 1);
    rb_define_method(rb_cMx, "**", mx_ewpow, 1);

    rb_define_singleton_method(rb_cMx, "arange", mx_sing_arange, -1);
    rb_define_singleton_method(rb_cMx, "linspace", mx_sing_linspace, -1);

    Init_random();
}
