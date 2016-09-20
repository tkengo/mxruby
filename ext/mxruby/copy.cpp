#include "mx.h"
#include "type.h"

void mxx_copy_shape(MX *src, MX *dest)
{
    // if (dest->shape != NULL) {
    //     MX_FREE(dest->shape);
    // }

    dest->dim   = src->dim;
    dest->size  = src->size;
    dest->shape = MX_ALLOC_N(size_t, src->dim);
    memcpy(dest->shape, src->shape, sizeof(size_t) * src->dim);
}

void mxx_copy_cast_elptr(MX *src, MX *dest, DTYPE cast_dtype)
{
    dest->dtype = cast_dtype;
    // if (dest->elptr == NULL) {
        dest->elptr = MX_ALLOC_N(char, dest->size * DTYPE_SIZES[dest->dtype]);
    // }
    // else if (DTYPE_SIZES[src->dtype] != DTYPE_SIZES[dest->dtype]) {
    //     MX_FREE(dest->elptr);
    //     dest->elptr = MX_ALLOC_N(char, dest->size * DTYPE_SIZES[dest->dtype]);
    // }

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
}

void mxx_copy_elptr(MX *src, MX *dest)
{
    mxx_copy_cast_elptr(src, dest, src->dtype);
}

void mxx_copy(MX *src, MX *dest)
{
    mxx_copy_shape(src, dest);
    mxx_copy_elptr(src, dest);
}

void mxx_copy_cast(MX *src, MX *dest, DTYPE new_dtype)
{
    mxx_copy_shape(src, dest);
    mxx_copy_cast_elptr(src, dest, new_dtype);
}

void mxx_copy_by_size(void *src, void *dest, size_t n, DTYPE dtype)
{
    switch (dtype) {
        case DTYPE_INT8:    mxt_copy((int8_t  *)src, (int8_t  *)dest, n); break;
        case DTYPE_INT16:   mxt_copy((int16_t *)src, (int16_t *)dest, n); break;
        case DTYPE_INT32:   mxt_copy((int32_t *)src, (int32_t *)dest, n); break;
        case DTYPE_INT64:   mxt_copy((int64_t *)src, (int64_t *)dest, n); break;
        case DTYPE_FLOAT32: mxt_copy((float   *)src, (float   *)dest, n); break;
        case DTYPE_FLOAT64: mxt_copy((double  *)src, (double  *)dest, n); break;
    }
}
