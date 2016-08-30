#include <ruby.h>
#include "mx.h"
#include <cblas.h>

VALUE rb_cMx;

static void mx_free(MX *mx)
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
    MX *mx = ALLOC(MX);
    mx->dim = 0;
    mx->size = 0;
    mx->dtype = DTYPE_FLOAT64;
    mx->shape = NULL;
    mx->elptr = NULL;

    return Data_Wrap_Struct(klass, 0, mx_free, mx);
}

void mx_type_rb_to_c(void *p, DTYPE dtype, VALUE *v)
{
    switch (dtype) {
        case DTYPE_INT32:
            *(int32_t *)p = FIX2INT(*v);
            break;
        case DTYPE_FLOAT32:
            *(float *)p = NUM2DBL(*v);
            break;
        case DTYPE_FLOAT64:
            *(double *)p = NUM2DBL(*v);
            break;
    }
}

void mx_type_c_to_rb(void *p, DTYPE dtype, VALUE *v)
{
    switch (dtype) {
        case DTYPE_INT32:
            *v = INT2FIX(*(int32_t *)p);
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
            for (int i = 0; i < NUM_DTYPE; i++) {
                if (SYM2ID(rb_dtype) == rb_intern(DTYPE_NAMES[i])) {
                    mx->dtype = i;
                    break;
                }
            }
        }
    }

    size_t dsize = DTYPE_SIZES[mx->dtype];

    mx->dim = RARRAY_LEN(shape);
    mx->size = 1;
    mx->shape = MX_ALLOC_N(size_t, mx->dim);

    for (int i = 0; i < mx->dim; i++) {
        mx->shape[i] = FIX2INT(RARRAY_AREF(shape, i));
        mx->size *= mx->shape[i];
    }
    mx->elptr = MX_ALLOC_N(char, dsize * mx->size);

    if (TYPE(initial_value) == T_ARRAY) {
        for (int i = 0; i < mx->size; i++) {
            VALUE v = RARRAY_AREF(initial_value, i);
            mx_type_rb_to_c(mx->elptr + i * dsize, mx->dtype, &v);
        }
    } else if (!NIL_P(initial_value)) {
        for (int i = 0; i < mx->size; i++) {
            mx_type_rb_to_c(mx->elptr + i * dsize, mx->dtype, &initial_value);
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

static VALUE mx_dot(VALUE self, VALUE target)
{
    int N = 2;
    MX *mx_self = DATA_PTR(self);
    void *c = MX_ALLOC_N(char, mx_self->shape[0] * mx_self->shape[1] * DTYPE_SIZES[4]);

    cblas_sgemm(
            CblasRowMajor, CblasNoTrans, CblasNoTrans,
            N, N, N,
            1, mx_self->elptr, N,
            MX_DATA_ELPTR(target), N,
            0, c, N
            );

    MX_FREE(mx_self->elptr);
    mx_self->elptr = c;
    return self;
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
    mx_type_c_to_rb(mx->elptr + pos * DTYPE_SIZES[mx->dtype], mx->dtype, &v);
    return v;
}

void _mx_to_a(VALUE *ary, size_t shape_index, size_t array_index, MX *mx)
{
    if (shape_index == mx->dim - 1) {
        for (int i = 0; i < mx->shape[shape_index]; i++) {
            void *p = mx->elptr + (i + array_index) * DTYPE_SIZES[mx->dtype];
            mx_type_c_to_rb(p, mx->dtype, &ary[i]);
        }
    } else {
        size_t skip = 1;
        for (int i = shape_index + 1; i < mx->dim; i++) {
            skip *= mx->shape[i];
        }

        for (int i = 0; i < mx->shape[shape_index]; i++) {
            VALUE *v = MX_ALLOC_N(VALUE, mx->shape[shape_index + 1]);
            _mx_to_a(v, shape_index + 1, skip * i + array_index, mx);
            ary[i] = rb_ary_new4(mx->shape[shape_index + 1], v);
        }
    }
}

static VALUE mx_to_a(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    VALUE *ary = MX_ALLOC_N(VALUE, mx->shape[0]);
    _mx_to_a(ary, 0, 0, mx);
    return rb_ary_new4(mx->shape[0], ary);
}

static VALUE mx_flatten(VALUE self)
{
    MX *mx = MX_DATA_PTR(self);
    VALUE *el = MX_ALLOC_N(VALUE, mx->size);
    for (int i = 0; i < mx->size; i++) {
        void *ref = mx->elptr + i * DTYPE_SIZES[4];
        el[i] = rb_float_new(*(float *)ref);
    }
    return rb_ary_new4(mx->size, el);
}

void Init_mxruby()
{
    rb_cMx = rb_define_class("MX", rb_cObject);

    rb_define_alloc_func(rb_cMx, mx_alloc);

    rb_define_protected_method(rb_cMx, "__set__", mx_initialize, 3);

    rb_define_method(rb_cMx, "shape", mx_shape, 0);
    rb_define_method(rb_cMx, "dim", mx_dim, 0);
    rb_define_method(rb_cMx, "size", mx_size, 0);
    rb_define_method(rb_cMx, "dot", mx_dot, 1);
    rb_define_method(rb_cMx, "[]=", mx_set, -1);
    rb_define_method(rb_cMx, "[]", mx_get, -1);
    rb_define_method(rb_cMx, "to_a", mx_to_a, 0);
    rb_define_method(rb_cMx, "flatten", mx_flatten, 0);

    Init_random();
}
