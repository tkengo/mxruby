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

void mxx_ewadd_scalar(MX *l, double r)
{
    switch (l->dtype) {
        case DTYPE_INT8:    mxt_ewadd_scalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   mxt_ewadd_scalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   mxt_ewadd_scalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   mxt_ewadd_scalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: mxt_ewadd_scalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: mxt_ewadd_scalar((double  *)l->elptr, r, l->size); break;
    }
}

void mxx_ewsub_scalar(MX *l, double r)
{
    switch (l->dtype) {
        case DTYPE_INT8:    mxt_ewsub_scalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   mxt_ewsub_scalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   mxt_ewsub_scalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   mxt_ewsub_scalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: mxt_ewsub_scalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: mxt_ewsub_scalar((double  *)l->elptr, r, l->size); break;
    }
}

void mxx_ewmul_scalar(MX *l, double r)
{
    switch (l->dtype) {
        case DTYPE_INT8:    mxt_ewmul_scalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   mxt_ewmul_scalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   mxt_ewmul_scalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   mxt_ewmul_scalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: mxt_ewmul_scalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: mxt_ewmul_scalar((double  *)l->elptr, r, l->size); break;
    }
}

void mxx_ewintpow_scalar(MX *l, int r)
{
    switch (l->dtype) {
        case DTYPE_INT8:    mxt_ewintpow_scalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   mxt_ewintpow_scalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   mxt_ewintpow_scalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   mxt_ewintpow_scalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: mxt_ewintpow_scalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: mxt_ewintpow_scalar((double  *)l->elptr, r, l->size); break;
    }
}

void mxx_ewpow_scalar(MX *l, double r)
{
    switch (l->dtype) {
        case DTYPE_FLOAT32: mxt_ewpow_scalar((float  *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: mxt_ewpow_scalar((double *)l->elptr, r, l->size); break;
    }
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

void mxx_arange(MX *mx, double start, double step)
{
    switch (mx->dtype) {
        case DTYPE_INT8:    mxt_arange((int8_t  *)mx->elptr, mx->size, (int8_t) start, (int8_t) step); break;
        case DTYPE_INT16:   mxt_arange((int16_t *)mx->elptr, mx->size, (int16_t)start, (int16_t)step); break;
        case DTYPE_INT32:   mxt_arange((int32_t *)mx->elptr, mx->size, (int32_t)start, (int32_t)step); break;
        case DTYPE_INT64:   mxt_arange((int64_t *)mx->elptr, mx->size, (int64_t)start, (int64_t)step); break;
        case DTYPE_FLOAT32: mxt_arange((float   *)mx->elptr, mx->size, (float)  start, (float)  step); break;
        case DTYPE_FLOAT64: mxt_arange((double  *)mx->elptr, mx->size, (double) start, (double) step); break;
    }
}
