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
/* #include <cblas.h> */
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

    /* cblas_sgemm( */
    /*         CblasRowMajor, CblasNoTrans, CblasNoTrans, */
    /*         N, N, N, */
    /*         1, mx->elptr, N, */
    /*         MX_DATA_ELPTR(target), N, */
    /*         0, c, N */
    /*         ); */

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
}

static VALUE mx_sum(int argc, VALUE *argv, VALUE self)
{
    MX *mx = MX_DATA_PTR(self);

    if (DTYPE_IS_INT(mx->dtype)) {
        long sum = (long)mxx_sum(mx);
        return INT2FIX(sum);
    } else {
        return rb_float_new(mxx_sum(mx));
    }
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

/**
 * Slice the `src` matrix, then the sliced matrix will be stored into the dest matrix.
 * @params src   Source matrix to be sliced
 * @params dest  Destination matrix
 * @params src_pos   Copy start position in the source matrix
 * @params count Copy element count
 * @params depth ?
 * @params idx
 * @params idx_len
 */
static void _mxs_slice(MX *src, MX *dest, VALUE idx, size_t idx_len, int depth, size_t size,
        size_t src_pos, size_t *dest_pos)
{
    if (depth == idx_len) {
        mxx_copy_by_size(
            MX_DATA_POS(src,  src_pos),
            MX_DATA_POS(dest, *dest_pos),
            size,
            dest->dtype
        );

        *dest_pos += size;
    } else {
        size_t skip = 1;
        for (int i = depth + 1; i < src->dim; i++) {
            skip *= src->shape[i];
        }

        long beg, len;
        VALUE v = RARRAY_AREF(idx, depth);
        if (FIXNUM_P(v)) {
            _mxs_slice(src, dest, idx, idx_len, depth + 1, skip, src_pos + FIX2INT(v) * skip, dest_pos);
        } else if (rb_range_beg_len(v, &beg, &len, src->shape[depth], 0) == Qtrue) {
            for (long i = 0; i < len; i++) {
                _mxs_slice(src, dest, idx, idx_len, depth + 1, skip, src_pos + (beg + i) * skip, dest_pos);
            }
        }
    }
}

static MX *mxs_slice(MX *src, VALUE idx, size_t idx_len)
{
    /**
     * mx = [ [ [1,1], [2,2], [3,3], [4,4] ],
     *        [ [0,0], [0,0], [0,0], [0,0] ],
     *        [ [5,5], [6,6], [7,7], [8,8] ] ], shape = [3, 4, 2]
     *
     * mx[0]    =   [ [1,1], [2,2], [3,3], [4,4] ],   shape = [4, 2]
     * mx[0..1] = [ [ [1,1], [2,2], [3,3], [4,4] ],
     *              [ [0,0], [0,0], [0,0], [0,0] ] ], shape = [2, 4, 2]
     *
     * mx[0, 0..1] = [ [1,1], [2,2] ],   shape = [2, 2]
     *
     * mx[0..1, 0]    = [ [1,1],
     *                    [0,0] ], shape = [2, 2]
     * mx[0..1, 0..1] = [ [ [1,1], [2,2] ],
     *                    [ [0,0], [0,0] ] ], shape = [2, 2, 2]
     *
     * mx[0..1, 0..1, 0] = [ [ 1, 2 ],
     *                       [ 0, 0 ] ], shape = [2, 2]
     *
     * mx = [ [1, 2, 3],
     *        [4, 5, 6],
     *        [7, 8, 9] ], shape = [3, 3]
     *
     * mx[0]         =   [1, 2, 3],   shape = [3]
     * mx[0..1]      = [ [1, 2, 3]
     *                   [4, 5, 6] ], shape = [2, 3]
     * mx[0..1, 0]   = [ 1
     *                   4 ],         shape = [2, 1]
     * mx[0..1, [0]] = [ [1]
       *                 [4] ],       shape = [2, 1]
     *
     * mx[0, 0]    =     1,  shape = nil
     * mx[0, 0..1] = [1, 2], shape = [2]
     *
     * mx[[0]]    = [ [1, 2, 3] ], shape = [1, 3]
     * mx[0..0]   = [ [1, 2, 3]
     *                [4, 5, 6] ], shape = [2, 3]
     * mx[[0, 1]] = [ [1, 2, 3]
     *                [4, 5, 6] ], shape = [2, 3]
     *
     * 引数がFIXNUMだったらdimが1個減る
     * 引数がrangeまたはarrayだったら次元は減らずにshapeがその長さになる
     */
    long beg, len;
    size_t new_dim = 0;
    size_t *tmp = MX_ALLOC_N(size_t, src->dim);
    for (int i = 0; i < src->dim; i++) {
        if (i < idx_len) {
            VALUE v = RARRAY_AREF(idx, i);
            if (rb_range_beg_len(v, &beg, &len, src->shape[i], 0) == Qtrue) {
                tmp[new_dim++] = len;
            }
        } else {
            tmp[new_dim++] = src->shape[i];
        }
    }

    VALUE *new_shape = MX_ALLOC_N(VALUE, new_dim);
    for (int i = 0; i < new_dim; i++) {
        new_shape[i] = INT2FIX(tmp[i]);
    }

    MX *new_mx = MX_ALLOC(MX);
    mxx_initialize_shape(new_mx, rb_ary_new4(new_dim, new_shape));
    new_mx->dtype = src->dtype;
    new_mx->elptr = MX_ALLOC_N(char, new_mx->size * DTYPE_SIZES[new_mx->dtype]);
    MX_FREE(tmp);

    size_t dest_pos = 0;
    _mxs_slice(src, new_mx, idx, idx_len, 0, 0, 0, &dest_pos);
}

static VALUE mx_get(int argc, VALUE *argv, VALUE self)
{
    VALUE idx;
    rb_scan_args(argc, argv, "0*", &idx);
    MX *mx = MX_DATA_PTR(self);

    size_t idx_len = RARRAY_LEN(idx);
    if (idx_len > mx->dim) {
        return Qnil;
    }

    MX *new_mx = mxs_slice(mx, idx, idx_len);
    return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, new_mx);

    return self;
}

static VALUE mx_ewadd(VALUE self, VALUE other)
{
    MX *mx = MX_DATA_PTR(self);
    MX *new_mx = MX_ALLOC(MX);

    if (IS_MX(other)) {
        MX *other_mx = MX_DATA_PTR(other);
        if (!mxx_is_same_shape(mx, other_mx)) {
            rb_raise(rb_eDataTypeError, "Cannot do addition operation between different shapes");
        }
        mxx_ewadd_array(mx, other_mx, new_mx);
    } else {
        if (TYPE(other) == T_FLOAT) {
            mxx_ewadd_dblscalar(mx, NUM2DBL(other), new_mx);
        } else {
            mxx_ewadd_intscalar(mx, FIX2INT(other), new_mx);
        }
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
            rb_raise(rb_eDataTypeError, "Cannot do subtraction operation between different shapes");
        }
        mxx_ewsub_array(mx, other_mx, new_mx);
    } else {
        if (TYPE(other) == T_FLOAT) {
            mxx_ewsub_dblscalar(mx, NUM2DBL(other), new_mx);
        } else {
            mxx_ewsub_intscalar(mx, FIX2INT(other), new_mx);
        }
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
            rb_raise(rb_eDataTypeError, "Cannot do multiplication operation between different shapes");
        }
        mxx_ewmul_array(mx, other_mx, new_mx);
    } else {
        if (TYPE(other) == T_FLOAT) {
            mxx_ewmul_dblscalar(mx, NUM2DBL(other), new_mx);
        } else {
            mxx_ewmul_intscalar(mx, FIX2INT(other), new_mx);
        }
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
            mxx_ewpow_array(mx, other_mx, new_mx);
        }
    } else {
        if (TYPE(other) == T_FIXNUM) {
            mxx_ewpow_intscalar(mx, FIX2INT(other), new_mx);
        } else {
            mxx_ewpow_dblscalar(mx, NUM2DBL(other), new_mx);
        }
    }

    return Data_Wrap_Struct(CLASS_OF(self), 0, mxx_free, new_mx);
}

static VALUE mx_sing_eye(int argc, VALUE *argv, VALUE klass)
{
    int row, col, dim = 2;
    VALUE rb_row, rb_col, rb_opt;
    rb_scan_args(argc, argv, "11:", &rb_row, &rb_col, &rb_opt);
    DTYPE dtype = mxx_dtype_from_opt(rb_opt, DTYPE_FLOAT64);

    VALUE *shape = MX_ALLOC_N(VALUE, dim);
    shape[0] = rb_row;
    if (NIL_P(rb_col)) {
        shape[1] = rb_row;
    } else {
        shape[1] = rb_col;
    }
    int count = MX_MIN(FIX2INT(shape[0]), FIX2INT(shape[1]));
    if (count < 0) {
        MX_FREE(shape);
        rb_raise(rb_eArgError, "Negative dimensions are not allowed");
    }

    MX *mx = mxx_initialize(rb_ary_new4(dim, shape), dtype);

    memset(mx->elptr, 0x00, mx->size * DTYPE_SIZES[mx->dtype]);
    mxx_eye(mx, count);
    MX_FREE(shape);
    return Data_Wrap_Struct(klass, 0, mxx_free, mx);;
}

static VALUE mx_sing_arange(int argc, VALUE *argv, VALUE klass)
{
    double start, stop, step;
    int start_type, stop_type, step_type;
    VALUE rb_start, rb_stop, rb_step, rb_opt;
    DTYPE dtype;

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
        MX *mx = mxx_initialize(INT2FIX(0), DTYPE_INT64);
        return Data_Wrap_Struct(klass, 0, mxx_free, mx);
    }

    // T_FIXNUM => FIXNUM_P
    if (step_type == T_FIXNUM && start_type == T_FIXNUM && stop_type == T_FIXNUM) {
        dtype = DTYPE_INT64;
    } else {
        dtype = DTYPE_FLOAT64;
    }

    size_t shape = (size_t)fabs(ceil((stop - start) / step));
    MX *mx = mxx_initialize(INT2FIX(shape), mxx_dtype_from_opt(rb_opt, dtype));
    mxx_linspace(mx, start, step);

    return Data_Wrap_Struct(klass, 0, mxx_free, mx);
}

static VALUE mx_sing_linspace(int argc, VALUE *argv, VALUE klass)
{
    VALUE rb_start, rb_stop, rb_num, rb_opt;
    rb_scan_args(argc, argv, "21:", &rb_start, &rb_stop, &rb_num, &rb_opt);

    if (NIL_P(rb_num)) {
        rb_num = INT2FIX(100);
    }

    if (FIX2INT(rb_num) < 0) {
        rb_raise(rb_eArgError, "Cannot specify negative number to the shape argument");
    }

    DTYPE dtype = mxx_dtype_from_opt(rb_opt, DTYPE_FLOAT64);
    MX *mx = mxx_initialize(rb_num, dtype);
    double start = NUM2DBL(rb_start);
    double stop  = NUM2DBL(rb_stop);
    double step  = (stop - start) * (1.0 / (double)(mx->size - 1));
    mxx_linspace(mx, start, step);

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
    rb_define_method(rb_cMx, "sum", mx_sum, -1);

    rb_define_method(rb_cMx, "empty?", mx_is_empty, 0);

    rb_define_method(rb_cMx, "[]=", mx_set, -1);
    rb_define_method(rb_cMx, "[]", mx_get, -1);
    rb_define_method(rb_cMx, "+", mx_ewadd, 1);
    rb_define_method(rb_cMx, "-", mx_ewsub, 1);
    rb_define_method(rb_cMx, "*", mx_ewmul, 1);
    rb_define_method(rb_cMx, "**", mx_ewpow, 1);

    rb_define_singleton_method(rb_cMx, "eye", mx_sing_eye, -1);
    rb_define_singleton_method(rb_cMx, "arange", mx_sing_arange, -1);
    rb_define_singleton_method(rb_cMx, "linspace", mx_sing_linspace, -1);

    Init_random();
}
