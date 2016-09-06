#include <ruby.h>
#include "mx.h"
#include "ewop.h"

MX *mxx_ewmul_array(MX *l, MX *r)
{
    void *new_ptr;
    switch (l->dtype) {
        case DTYPE_INT8:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul_array((int8_t *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul_array((int8_t *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul_array((int8_t *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul_array((int8_t *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul_array((int8_t *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul_array((int8_t *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_INT16:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul_array((int16_t *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul_array((int16_t *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul_array((int16_t *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul_array((int16_t *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul_array((int16_t *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul_array((int16_t *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_INT32:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul_array((int32_t *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul_array((int32_t *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul_array((int32_t *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul_array((int32_t *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul_array((int32_t *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul_array((int32_t *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_INT64:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul_array((int64_t *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul_array((int64_t *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul_array((int64_t *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul_array((int64_t *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul_array((int64_t *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul_array((int64_t *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_FLOAT32:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul_array((float *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul_array((float *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul_array((float *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul_array((float *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul_array((float *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul_array((float *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_FLOAT64:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul_array((double *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul_array((double *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul_array((double *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul_array((double *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul_array((double *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul_array((double *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
    }

    MX *new_mx = mxx_copy_shape(l);
    new_mx->dtype = l->dtype > r->dtype ? l->dtype : r->dtype;
    new_mx->elptr = new_ptr;

    return new_mx;
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

MX *mxx_cast_copy(MX *src, DTYPE cast_dtype)
{
    MX *dest = mxx_copy_shape(src);
    dest->dtype = cast_dtype;
    dest->elptr = MX_ALLOC_N(char, dest->size * DTYPE_SIZES[cast_dtype]);

    switch (dest->dtype) {
        case DTYPE_INT8:
            switch (src->dtype) {
                case DTYPE_INT8:    memcpy(dest->elptr, src->elptr, dest->size * sizeof(int8_t)); break;
                case DTYPE_INT16:   mxt_cast((int8_t *)dest->elptr, (int16_t *)src->elptr, dest->size); break;
                case DTYPE_INT32:   mxt_cast((int8_t *)dest->elptr, (int32_t *)src->elptr, dest->size); break;
                case DTYPE_INT64:   mxt_cast((int8_t *)dest->elptr, (int64_t *)src->elptr, dest->size); break;
                case DTYPE_FLOAT32: mxt_cast((int8_t *)dest->elptr, (float   *)src->elptr, dest->size); break;
                case DTYPE_FLOAT64: mxt_cast((int8_t *)dest->elptr, (double  *)src->elptr, dest->size); break;
            }
            break;
        case DTYPE_INT16:
            switch (src->dtype) {
                case DTYPE_INT8:    mxt_cast((int16_t *)dest->elptr, (int8_t  *)src->elptr, dest->size); break;
                case DTYPE_INT16:   memcpy(dest->elptr, src->elptr, dest->size * sizeof(int16_t)); break;
                case DTYPE_INT32:   mxt_cast((int16_t *)dest->elptr, (int32_t *)src->elptr, dest->size); break;
                case DTYPE_INT64:   mxt_cast((int16_t *)dest->elptr, (int64_t *)src->elptr, dest->size); break;
                case DTYPE_FLOAT32: mxt_cast((int16_t *)dest->elptr, (float   *)src->elptr, dest->size); break;
                case DTYPE_FLOAT64: mxt_cast((int16_t *)dest->elptr, (double  *)src->elptr, dest->size); break;
            }
            break;
        case DTYPE_INT32:
            switch (src->dtype) {
                case DTYPE_INT8:    mxt_cast((int32_t *)dest->elptr, (int8_t  *)src->elptr, dest->size); break;
                case DTYPE_INT16:   mxt_cast((int32_t *)dest->elptr, (int16_t *)src->elptr, dest->size); break;
                case DTYPE_INT32:   memcpy(dest->elptr, src->elptr, dest->size * sizeof(int32_t)); break;
                case DTYPE_INT64:   mxt_cast((int32_t *)dest->elptr, (int64_t *)src->elptr, dest->size); break;
                case DTYPE_FLOAT32: mxt_cast((int32_t *)dest->elptr, (float   *)src->elptr, dest->size); break;
                case DTYPE_FLOAT64: mxt_cast((int32_t *)dest->elptr, (double  *)src->elptr, dest->size); break;
            }
            break;
        case DTYPE_INT64:
            switch (src->dtype) {
                case DTYPE_INT8:    mxt_cast((int64_t *)dest->elptr, (int8_t  *)src->elptr, dest->size); break;
                case DTYPE_INT16:   mxt_cast((int64_t *)dest->elptr, (int16_t *)src->elptr, dest->size); break;
                case DTYPE_INT32:   mxt_cast((int64_t *)dest->elptr, (int32_t *)src->elptr, dest->size); break;
                case DTYPE_INT64:   memcpy(dest->elptr, src->elptr, dest->size * sizeof(int64_t)); break;
                case DTYPE_FLOAT32: mxt_cast((int64_t *)dest->elptr, (float   *)src->elptr, dest->size); break;
                case DTYPE_FLOAT64: mxt_cast((int64_t *)dest->elptr, (double  *)src->elptr, dest->size); break;
            }
            break;
        case DTYPE_FLOAT32:
            switch (src->dtype) {
                case DTYPE_INT8:    mxt_cast((float *)dest->elptr, (int8_t  *)src->elptr, dest->size); break;
                case DTYPE_INT16:   mxt_cast((float *)dest->elptr, (int16_t *)src->elptr, dest->size); break;
                case DTYPE_INT32:   mxt_cast((float *)dest->elptr, (int32_t *)src->elptr, dest->size); break;
                case DTYPE_INT64:   mxt_cast((float *)dest->elptr, (int64_t *)src->elptr, dest->size); break;
                case DTYPE_FLOAT32: memcpy(dest->elptr, src->elptr, dest->size * sizeof(float)); break;
                case DTYPE_FLOAT64: mxt_cast((float *)dest->elptr, (double  *)src->elptr, dest->size); break;
            }
            break;
        case DTYPE_FLOAT64:
            switch (src->dtype) {
                case DTYPE_INT8:    mxt_cast((double *)dest->elptr, (int8_t  *)src->elptr, dest->size); break;
                case DTYPE_INT16:   mxt_cast((double *)dest->elptr, (int16_t *)src->elptr, dest->size); break;
                case DTYPE_INT32:   mxt_cast((double *)dest->elptr, (int32_t *)src->elptr, dest->size); break;
                case DTYPE_INT64:   mxt_cast((double *)dest->elptr, (int64_t *)src->elptr, dest->size); break;
                case DTYPE_FLOAT32: mxt_cast((double *)dest->elptr, (float   *)src->elptr, dest->size); break;
                case DTYPE_FLOAT64: memcpy(dest->elptr, src->elptr, dest->size * sizeof(double));; break;
            }
            break;
    }

    return dest;
}
