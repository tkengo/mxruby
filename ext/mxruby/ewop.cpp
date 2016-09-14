#include "mx.h"
#include "ewop.h"

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

void mxx_ewpow_scalar(MX *l, int r)
{
    switch (l->dtype) {
        case DTYPE_INT8:    mxt_ewpow_scalar((int8_t  *)l->elptr, r, l->size); break;
        case DTYPE_INT16:   mxt_ewpow_scalar((int16_t *)l->elptr, r, l->size); break;
        case DTYPE_INT32:   mxt_ewpow_scalar((int32_t *)l->elptr, r, l->size); break;
        case DTYPE_INT64:   mxt_ewpow_scalar((int64_t *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT32: mxt_ewpow_scalar((float   *)l->elptr, r, l->size); break;
        case DTYPE_FLOAT64: mxt_ewpow_scalar((double  *)l->elptr, r, l->size); break;
    }
}
