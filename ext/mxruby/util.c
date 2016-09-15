#include "mx.h"

DTYPE mxx_dtype_from_symbol(VALUE dtype)
{
    for (int i = 0; i < NUM_DTYPE; i++) {
        if (SYM2ID(dtype) == rb_intern(DTYPE_NAMES[i])) {
            return i;
        }
    }

    return DTYPE_UNKNOWN;
}

DTYPE mxx_dtype_from_opt(VALUE opt)
{
    if (!NIL_P(opt)) {
        VALUE dtype = rb_hash_aref(opt, ID2SYM(rb_intern("dtype")));
        if (!NIL_P(dtype)) {
            return mxx_dtype_from_symbol(dtype);
        }
    }

    return DTYPE_UNKNOWN;
}

bool mxx_is_same_shape(MX *m1, MX *m2)
{
    if (m1->dim != m2->dim) {
        return false;
    }

    for (size_t d = 0; d < m1->dim; d++) {
        if (m1->shape[d] != m2->shape[d]) {
            return false;
        }
    }

    return true;
}

void mxx_rb_to_c(void *p, DTYPE dtype, VALUE v)
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

VALUE mxx_c_to_rb(void *p, DTYPE dtype)
{
    switch (dtype) {
        case DTYPE_INT8:
            return INT2FIX(*(int8_t *)p);
        case DTYPE_INT16:
            return INT2FIX(*(int16_t *)p);
        case DTYPE_INT32:
            return INT2FIX(*(int32_t *)p);
        case DTYPE_INT64:
            return INT2FIX(*(int64_t *)p);
        case DTYPE_FLOAT32:
            return rb_float_new(*(float *)p);
        case DTYPE_FLOAT64:
            return rb_float_new(*(double *)p);
        default:
            return INT2FIX(*(int64_t *)p);
    }
}
