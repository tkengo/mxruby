/**
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

VALUE rb_cMx;
VALUE rb_eDataTypeError;

MX *mxx_initialize(VALUE shape, DTYPE dtype)
{
    MX *mx = MX_ALLOC(MX);
    mxx_setup(mx, dtype);
    mxx_initialize_shape(mx, shape);
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
    mx->elptr = MX_ALLOC_N(char, DTYPE_SIZES[mx->dtype] * mx->size);
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

void mx_type_rb_to_c(void *p, DTYPE dtype, VALUE v)
{
    switch (dtype) {
        case DTYPE_INT8:
            *(int8_t *)p = FIX2INT(v);
            break;
        case DTYPE_INT16:
            *(int16_t *)p = FIX2INT(v);
            break;
        case DTYPE_INT32:
            *(int32_t *)p = FIX2INT(v);
            break;
        case DTYPE_INT64:
            *(int64_t *)p = FIX2INT(v);
            break;
        case DTYPE_FLOAT32:
            *(float *)p = NUM2DBL(v);
            break;
        case DTYPE_FLOAT64:
            *(double *)p = NUM2DBL(v);
            break;
    }
}

void mx_type_c_to_rb(void *p, DTYPE dtype, VALUE *v)
{
    switch (dtype) {
        case DTYPE_INT8:
            *v = INT2FIX(*(int8_t *)p);
            break;
        case DTYPE_INT16:
            *v = INT2FIX(*(int16_t *)p);
            break;
        case DTYPE_INT32:
            *v = INT2FIX(*(int32_t *)p);
            break;
        case DTYPE_INT64:
            *v = INT2FIX(*(int64_t *)p);
            break;
        case DTYPE_FLOAT32:
            *v = rb_float_new(*(float *)p);
            break;
        case DTYPE_FLOAT64:
            *v = rb_float_new(*(double *)p);
            break;
    }
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

    if (TYPE(initial_value) == T_ARRAY) {
        for (int i = 0; i < mx->size; i++) {
            VALUE v = RARRAY_AREF(initial_value, i);
            mx_type_rb_to_c(mx->elptr + i * DTYPE_SIZES[mx->dtype], mx->dtype, v);
        }
    } else if (!NIL_P(initial_value)) {
        for (int i = 0; i < mx->size; i++) {
            mx_type_rb_to_c(mx->elptr + i * DTYPE_SIZES[mx->dtype], mx->dtype, initial_value);
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
    /* VALUE rb_new_dtype, rb_opt; */
    /* rb_scan_args(argc, argv, "1:", &rb_new_dtype, &rb_opt); */
    /*  */
    /* MX *src = MX_DATA_PTR(self); */
    /* MX *dest = MX_INIT_D(src->shape, new_dtype); */
    /* DTYPE new_dtype = mxx_dtype_from_symbol(rb_new_dtype); */
    /*  */
    /* if (src->dtype == new_dtype) { */
    /*     memcpy(dest->elptr, src->elptr, dest->size * DTYPE_SIZES[new_dtype]); */
    /* } else { */
    /* } */
    return self;
}

static void mxs_to_a(VALUE *ary, size_t shape_index, size_t array_index, MX *mx)
{
    if (shape_index == mx->dim - 1) {
        for (int i = 0; i < mx->shape[shape_index]; i++) {
            void *p = (char *)mx->elptr + (i + array_index) * DTYPE_SIZES[mx->dtype];
            mx_type_c_to_rb(p, mx->dtype, &ary[i]);
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
        mx_type_c_to_rb((char *)mx->elptr + i * dsize, mx->dtype, &el[i]);
    }
    return rb_ary_new4(mx->size, el);
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

    VALUE v;
    mx_type_c_to_rb((char *)mx->elptr + pos * DTYPE_SIZES[mx->dtype], mx->dtype, &v);
    return v;
}

static VALUE mx_ewmul(VALUE self, VALUE other)
{
    MX *mx = MX_DATA_PTR(self);

    int type = TYPE(other);
    if (mx->dim == 1) {
        if (IS_MX(other)) {
            MX *other_mx = MX_DATA_PTR(other);
            if (mx->size != other_mx->size) {
                rb_raise(rb_eDataTypeError, "Cannot do multiply operation between shape of [ %ld ] and [ %ld ]", mx->size, other_mx->size);
            }

            MX *new_mx = mxx_ewmul(mx, other_mx);
            return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, new_mx);
        } else {
        }
        /* if (type == T_FIXNUM || type == T_FLOAT) { */
        /*     if (mx->dtype == DTYPE_FLOAT64) { */
        /*         cblas_dscal(mx->size, NUM2DBL(other), mx->elptr, 1); */
        /*     } else if (mx->dtype == DTYPE_FLOAT32) { */
        /*         cblas_sscal(mx->size, NUM2DBL(other), mx->elptr, 1); */
        /*     } else { */
        /*         if (type == T_FIXNUM) { */
        /*             int v = FIX2INT(other); */
        /*             size_t dsize = DTYPE_SIZES[mx->dtype]; */
        /*             for (int i = 0; i < mx->size; i++) { */
        /*                 switch (mx->dtype) { */
        /*                     case DTYPE_INT8: */
        /*                         *(int8_t *)(mx->elptr + i * dsize) *= v; */
        /*                         break; */
        /*                     case DTYPE_INT16: */
        /*                         *(int16_t *)(mx->elptr + i * dsize) *= v; */
        /*                         break; */
        /*                     case DTYPE_INT32: */
        /*                         *(int32_t *)(mx->elptr + i * dsize) *= v; */
        /*                         break; */
        /*                     case DTYPE_INT64: */
        /*                         *(int64_t *)(mx->elptr + i * dsize) *= v; */
        /*                         break; */
        /*                 } */
        /*             } */
        /*         } else if (type == T_FLOAT) { */
        /*             mxx_cast(mx, DTYPE_FLOAT64); */
        /*             cblas_dscal(mx->size, NUM2DBL(other), mx->elptr, 1); */
        /*         } */
        /*     } */
        /* } else if (IS_MX(other)) { */
        /*     MX *other_mx = MX_DATA_PTR(other); */
        /*     if (mx->size != other_mx->size) { */
        /*         rb_raise(rb_eDataTypeError, "Cannot do multiply operation between shape of [ %ld ] and [ %ld ]", mx->size, other_mx->size); */
        /*     } */
        /* } */
    }

    return self;
}

static VALUE mx_sing_arange(int argc, VALUE *argv, VALUE klass)
{
    DTYPE dtype;
    double start, stop;
    int start_type, stop_type;
    VALUE rb_start, rb_stop, rb_step, rb_opt;
    rb_scan_args(argc, argv, "12:", &rb_start, &rb_stop, &rb_step, &rb_opt);

    if (NIL_P(rb_stop)) {
        start = 0;
        stop  = NUM2DBL(rb_start);
        start_type = T_FIXNUM;
        stop_type  = TYPE(rb_start);
    } else {
        start = NUM2DBL(rb_start);
        stop  = NUM2DBL(rb_stop);
        start_type = TYPE(rb_start);
        stop_type  = TYPE(rb_stop);
    }
    size_t step = NIL_P(rb_step) ? step = 1 : FIX2INT(rb_step);

    if ((start_type == T_FIXNUM || start_type == T_BIGNUM) &&
        (stop_type  == T_FIXNUM || stop_type  == T_BIGNUM)) {
        dtype = DTYPE_INT64;
    } else {
        dtype = DTYPE_FLOAT64;
    }

    size_t shape = (size_t)ceil((stop - start) / (double)step);
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
    VALUE rb_start, rb_end, rb_shape, rb_opt;
    rb_scan_args(argc, argv, "21:", &rb_start, &rb_end, &rb_shape, &rb_opt);

    if (NIL_P(rb_shape)) {
        rb_shape = INT2FIX(100);
    }

    MX *mx = MX_INIT_D(rb_shape, DTYPE_FLOAT64);
    double start = NUM2DBL(rb_start);
    double end   = NUM2DBL(rb_end);
    double step  = (end - start) * (1.0 / (double)(mx->size - 1));

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

    rb_define_method(rb_cMx, "[]=", mx_set, -1);
    rb_define_method(rb_cMx, "[]", mx_get, -1);
    rb_define_method(rb_cMx, "*", mx_ewmul, 1);

    rb_define_singleton_method(rb_cMx, "arange", mx_sing_arange, -1);
    rb_define_singleton_method(rb_cMx, "linspace", mx_sing_linspace, -1);

    Init_random();
}
