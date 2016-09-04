#include <ruby.h>
#include "mx.h"
#include "ewop.h"

MX *mxx_ewmul(MX *l, MX *r)
{
    void *new_ptr;
    size_t new_dtype;

    switch (l->dtype) {
        case DTYPE_INT8:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul((int8_t *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul((int8_t *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul((int8_t *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul((int8_t *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul((int8_t *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul((int8_t *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_INT16:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul((int16_t *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul((int16_t *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul((int16_t *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul((int16_t *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul((int16_t *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul((int16_t *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_INT32:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul((int32_t *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul((int32_t *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul((int32_t *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul((int32_t *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul((int32_t *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul((int32_t *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_INT64:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul((int64_t *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul((int64_t *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul((int64_t *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul((int64_t *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul((int64_t *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul((int64_t *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_FLOAT32:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul((float *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul((float *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul((float *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul((float *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul((float *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul((float *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
        case DTYPE_FLOAT64:
            switch (r->dtype) {
                case DTYPE_INT8:    new_ptr = mxt_ewmul((double *)l->elptr, (int8_t  *)r->elptr, l->size); break;
                case DTYPE_INT16:   new_ptr = mxt_ewmul((double *)l->elptr, (int16_t *)r->elptr, l->size); break;
                case DTYPE_INT32:   new_ptr = mxt_ewmul((double *)l->elptr, (int32_t *)r->elptr, l->size); break;
                case DTYPE_INT64:   new_ptr = mxt_ewmul((double *)l->elptr, (int64_t *)r->elptr, l->size); break;
                case DTYPE_FLOAT32: new_ptr = mxt_ewmul((double *)l->elptr, (float   *)r->elptr, l->size); break;
                case DTYPE_FLOAT64: new_ptr = mxt_ewmul((double *)l->elptr, (double  *)r->elptr, l->size); break;
            }
            break;
    }

    MX *new_mx = MX_ALLOC(MX);
    mxx_copy_shape(new_mx, l);
    new_mx->dtype = l->dtype > r->dtype ? l->dtype : r->dtype;
    new_mx->elptr = new_ptr;

    return new_mx;
}
