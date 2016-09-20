/*
 * Copyright (c) 2016 Kengo Tateishi (@tkengo)
 * Licensed under MIT license.
 *   http://www.opensource.org/licenses/mit-license.php
 *
 * This is implementation of operations for each types. Operations are implemented by C++ template.
 */
#include "mx.h"
#include "type.h"

#define EWOP_ARRAY_SWITCH_DISPATCH(method,type) \
    switch (r->dtype) { \
        case DTYPE_INT8:    new_ptr = method((type *)l->elptr, (int8_t  *)r->elptr, l->size); break; \
        case DTYPE_INT16:   new_ptr = method((type *)l->elptr, (int16_t *)r->elptr, l->size); break; \
        case DTYPE_INT32:   new_ptr = method((type *)l->elptr, (int32_t *)r->elptr, l->size); break; \
        case DTYPE_INT64:   new_ptr = method((type *)l->elptr, (int64_t *)r->elptr, l->size); break; \
        case DTYPE_FLOAT32: new_ptr = method((type *)l->elptr, (float   *)r->elptr, l->size); break; \
        case DTYPE_FLOAT64: new_ptr = method((type *)l->elptr, (double  *)r->elptr, l->size); break; \
    } break

/*
 * Definition of element-wise operations between array and array.
 *
 * These methods have folloing signature.
 *   mxx_ew[op-name]_array(MX *l, MX *r, MX *out);
 *
 * op-name:
 *   There are 5 operation types.
 *   1. addition       (shorthand: add)
 *   2. subtraction    (shorthand: sub)
 *   3. multiplication (shorthand: mul)
 *   4. division       (shorthand: div)
 *   5. power          (shorthand: pow)
 *
 * For power operation, because pow method of libm is very slow, so MxRuby implements power
 * operation for integer value in order to improvement execution time. And only if exponent is
 * floating point number, MxRuby uses pow method of libm.
 *
 * arguments:
 *   First two arguments indicate a left operand and a right operand. The result outputs to the
 *   last argument.
 *
 * Basicaly, the output mxarray has dtype which the bigger one from left mxarray's dtype or right
 * mxarray's dtype.
 */
void mxx_ewadd_array(MX *l, MX *r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewadd_array, int8_t);
        case DTYPE_INT16:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewadd_array, int16_t);
        case DTYPE_INT32:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewadd_array, int32_t);
        case DTYPE_INT64:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewadd_array, int64_t);
        case DTYPE_FLOAT32: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewadd_array, float);
        case DTYPE_FLOAT64: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewadd_array, double);
    }

    mxx_copy_shape(l, out);
    out->dtype = l->dtype > r->dtype ? l->dtype : r->dtype;
    out->elptr = new_ptr;
}
void mxx_ewsub_array(MX *l, MX *r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewsub_array, int8_t);
        case DTYPE_INT16:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewsub_array, int16_t);
        case DTYPE_INT32:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewsub_array, int32_t);
        case DTYPE_INT64:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewsub_array, int64_t);
        case DTYPE_FLOAT32: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewsub_array, float);
        case DTYPE_FLOAT64: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewsub_array, double);
    }

    mxx_copy_shape(l, out);
    out->dtype = l->dtype > r->dtype ? l->dtype : r->dtype;
    out->elptr = new_ptr;
}
void mxx_ewmul_array(MX *l, MX *r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewmul_array, int8_t);
        case DTYPE_INT16:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewmul_array, int16_t);
        case DTYPE_INT32:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewmul_array, int32_t);
        case DTYPE_INT64:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewmul_array, int64_t);
        case DTYPE_FLOAT32: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewmul_array, float);
        case DTYPE_FLOAT64: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewmul_array, double);
    }

    mxx_copy_shape(l, out);
    out->dtype = l->dtype > r->dtype ? l->dtype : r->dtype;
    out->elptr = new_ptr;
}
void mxx_ewpow_array(MX *l, MX *r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewpow_array, int8_t);
        case DTYPE_INT16:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewpow_array, int16_t);
        case DTYPE_INT32:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewpow_array, int32_t);
        case DTYPE_INT64:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewpow_array, int64_t);
        case DTYPE_FLOAT32: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewpow_array, float);
        case DTYPE_FLOAT64: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewpow_array, double);
    }

    mxx_copy_shape(l, out);
    out->dtype = DTYPE_FLOAT64;
    out->elptr = new_ptr;
}
void mxx_ewintpow_array(MX *l, MX *r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewintpow_array, int8_t);
        case DTYPE_INT16:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewintpow_array, int16_t);
        case DTYPE_INT32:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewintpow_array, int32_t);
        case DTYPE_INT64:   EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewintpow_array, int64_t);
        case DTYPE_FLOAT32: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewintpow_array, float);
        case DTYPE_FLOAT64: EWOP_ARRAY_SWITCH_DISPATCH(mxt_ewintpow_array, double);
    }

    mxx_copy_shape(l, out);
    out->dtype = l->dtype;
    out->elptr = new_ptr;
}

/*
 * Definition of element-wise operations between array and scalar.
 *
 * These methods have folloing signature.
 *   mxx_ew[op-name]_intscalar(MX *l, long   *r, MX *out);
 *   mxx_ew[op-name]_dblscalar(MX *l, double *r, MX *out);
 *
 * op-name is same as array and array operation.
 *
 * arguments:
 *   First two arguments indicate a left operand and a right operand. The result outputs to the
 *   last argument.
 *
 * An output mxarray has float64 dtype if a scalar value is floating point number because the
 * result of each operation between any types and float number is also float number. But if a
 * scalar value is not float, a new mxaray has dtype same as a left operand.
 * So MxRuby must switch the invoked methods whether if a scalar value is float or not.
 */
void mxx_ewadd_intscalar(MX *l, long r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    new_ptr = mxt_ewadd_scalar((int8_t  *)l->elptr, (int8_t )r, l->size); break;
        case DTYPE_INT16:   new_ptr = mxt_ewadd_scalar((int16_t *)l->elptr, (int16_t)r, l->size); break;
        case DTYPE_INT32:   new_ptr = mxt_ewadd_scalar((int32_t *)l->elptr, (int32_t)r, l->size); break;
        case DTYPE_INT64:   new_ptr = mxt_ewadd_scalar((int64_t *)l->elptr, (int64_t)r, l->size); break;
        case DTYPE_FLOAT32: new_ptr = mxt_ewadd_scalar((float   *)l->elptr, (float  )r, l->size); break;
        case DTYPE_FLOAT64: new_ptr = mxt_ewadd_scalar((double  *)l->elptr, (double )r, l->size); break;
    }

    mxx_copy_shape(l, out);
    out->dtype = l->dtype;
    out->elptr = new_ptr;
}
void mxx_ewadd_dblscalar(MX *l, double r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    new_ptr = mxt_ewadd_scalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   new_ptr = mxt_ewadd_scalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   new_ptr = mxt_ewadd_scalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   new_ptr = mxt_ewadd_scalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: new_ptr = mxt_ewadd_scalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: new_ptr = mxt_ewadd_scalar((double  *)l->elptr, r, l->size); break;
    }

    mxx_copy_shape(l, out);
    out->dtype = DTYPE_FLOAT64;
    out->elptr = new_ptr;
}
void mxx_ewsub_intscalar(MX *l, long r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    new_ptr = mxt_ewsub_scalar((int8_t  *)l->elptr, (int8_t )r, l->size); break;
        case DTYPE_INT16:   new_ptr = mxt_ewsub_scalar((int16_t *)l->elptr, (int16_t)r, l->size); break;
        case DTYPE_INT32:   new_ptr = mxt_ewsub_scalar((int32_t *)l->elptr, (int32_t)r, l->size); break;
        case DTYPE_INT64:   new_ptr = mxt_ewsub_scalar((int64_t *)l->elptr, (int64_t)r, l->size); break;
        case DTYPE_FLOAT32: new_ptr = mxt_ewsub_scalar((float   *)l->elptr, (float  )r, l->size); break;
        case DTYPE_FLOAT64: new_ptr = mxt_ewsub_scalar((double  *)l->elptr, (double )r, l->size); break;
    }

    mxx_copy_shape(l, out);
    out->dtype = l->dtype;
    out->elptr = new_ptr;
}
void mxx_ewsub_dblscalar(MX *l, double r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    new_ptr = mxt_ewsub_scalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   new_ptr = mxt_ewsub_scalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   new_ptr = mxt_ewsub_scalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   new_ptr = mxt_ewsub_scalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: new_ptr = mxt_ewsub_scalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: new_ptr = mxt_ewsub_scalar((double  *)l->elptr, r, l->size); break;
    }

    mxx_copy_shape(l, out);
    out->dtype = DTYPE_FLOAT64;
    out->elptr = new_ptr;
}
void mxx_ewmul_intscalar(MX *l, long r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    new_ptr = mxt_ewmul_scalar((int8_t  *)l->elptr, (int8_t )r, l->size); break;
        case DTYPE_INT16:   new_ptr = mxt_ewmul_scalar((int16_t *)l->elptr, (int16_t)r, l->size); break;
        case DTYPE_INT32:   new_ptr = mxt_ewmul_scalar((int32_t *)l->elptr, (int32_t)r, l->size); break;
        case DTYPE_INT64:   new_ptr = mxt_ewmul_scalar((int64_t *)l->elptr, (int64_t)r, l->size); break;
        case DTYPE_FLOAT32: new_ptr = mxt_ewmul_scalar((float   *)l->elptr, (float  )r, l->size); break;
        case DTYPE_FLOAT64: new_ptr = mxt_ewmul_scalar((double  *)l->elptr, (double )r, l->size); break;
    }

    mxx_copy_shape(l, out);
    out->dtype = l->dtype;
    out->elptr = new_ptr;
}
void mxx_ewmul_dblscalar(MX *l, double r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    new_ptr = mxt_ewmul_scalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   new_ptr = mxt_ewmul_scalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   new_ptr = mxt_ewmul_scalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   new_ptr = mxt_ewmul_scalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: new_ptr = mxt_ewmul_scalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: new_ptr = mxt_ewmul_scalar((double  *)l->elptr, r, l->size); break;
    }

    mxx_copy_shape(l, out);
    out->dtype = DTYPE_FLOAT64;
    out->elptr = new_ptr;
}
void mxx_ewpow_intscalar(MX *l, long r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    new_ptr = mxt_ewpow_intscalar((int8_t  *)l->elptr, (int8_t )r, l->size); break;
        case DTYPE_INT16:   new_ptr = mxt_ewpow_intscalar((int16_t *)l->elptr, (int16_t)r, l->size); break;
        case DTYPE_INT32:   new_ptr = mxt_ewpow_intscalar((int32_t *)l->elptr, (int32_t)r, l->size); break;
        case DTYPE_INT64:   new_ptr = mxt_ewpow_intscalar((int64_t *)l->elptr, (int64_t)r, l->size); break;
        case DTYPE_FLOAT32: new_ptr = mxt_ewpow_intscalar((float   *)l->elptr, (float  )r, l->size); break;
        case DTYPE_FLOAT64: new_ptr = mxt_ewpow_intscalar((double  *)l->elptr, (double )r, l->size); break;
    }

    mxx_copy_shape(l, out);
    out->dtype = l->dtype;
    out->elptr = new_ptr;
}
void mxx_ewpow_dblscalar(MX *l, double r, MX *out)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:    new_ptr = mxt_ewpow_dblscalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   new_ptr = mxt_ewpow_dblscalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   new_ptr = mxt_ewpow_dblscalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   new_ptr = mxt_ewpow_dblscalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: new_ptr = mxt_ewpow_dblscalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: new_ptr = mxt_ewpow_dblscalar((double  *)l->elptr, r, l->size); break;
    }

    mxx_copy_shape(l, out);
    out->dtype = DTYPE_FLOAT64;
    out->elptr = new_ptr;
}

double mxx_sum(MX *mx)
{
    switch (mx->dtype) {
        case DTYPE_INT8:    return mxt_sum((int8_t  *)mx->elptr, mx->size);
        case DTYPE_INT16:   return mxt_sum((int16_t *)mx->elptr, mx->size);
        case DTYPE_INT32:   return mxt_sum((int32_t *)mx->elptr, mx->size);
        case DTYPE_INT64:   return mxt_sum((int64_t *)mx->elptr, mx->size);
        case DTYPE_FLOAT32: return mxt_sum((float   *)mx->elptr, mx->size);
        case DTYPE_FLOAT64: return mxt_sum((double  *)mx->elptr, mx->size);
        default: return 0;
    }
}

void mxx_linspace(MX *mx, double start, double step)
{
    switch (mx->dtype) {
        case DTYPE_INT8:    mxt_linspace((int8_t  *)mx->elptr, mx->size, (int8_t) start, (int8_t) step); break;
        case DTYPE_INT16:   mxt_linspace((int16_t *)mx->elptr, mx->size, (int16_t)start, (int16_t)step); break;
        case DTYPE_INT32:   mxt_linspace((int32_t *)mx->elptr, mx->size, (int32_t)start, (int32_t)step); break;
        case DTYPE_INT64:   mxt_linspace((int64_t *)mx->elptr, mx->size, (int64_t)start, (int64_t)step); break;
        case DTYPE_FLOAT32: mxt_linspace((float   *)mx->elptr, mx->size, (float)  start, (float)  step); break;
        case DTYPE_FLOAT64: mxt_linspace((double  *)mx->elptr, mx->size, (double) start, (double) step); break;
    }
}

void mxx_eye(MX *mx, size_t n)
{
    switch (mx->dtype) {
        case DTYPE_INT8:    mxt_eye((int8_t  *)mx->elptr, n); break;
        case DTYPE_INT16:   mxt_eye((int16_t *)mx->elptr, n); break;
        case DTYPE_INT32:   mxt_eye((int32_t *)mx->elptr, n); break;
        case DTYPE_INT64:   mxt_eye((int64_t *)mx->elptr, n); break;
        case DTYPE_FLOAT32: mxt_eye((float   *)mx->elptr, n); break;
        case DTYPE_FLOAT64: mxt_eye((double  *)mx->elptr, n); break;
    }
}
