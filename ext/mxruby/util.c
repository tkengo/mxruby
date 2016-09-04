#include <ruby.h>
#include "mx.h"

void mxx_copy_shape(MX *dest, MX *src)
{
    dest->dim   = src->dim;
    dest->size  = src->size;
    dest->shape = MX_ALLOC_N(size_t, src->dim);
    memcpy(dest->shape, src->shape, sizeof(size_t) * src->dim);
}

MX *mxx_cast_copy(MX *src, DTYPE dtype)
{
    MX *dest = MX_ALLOC(MX);
    dest->dim   = src->dim;
    dest->size  = src->size;
    dest->dtype = src->dtype;

    dest->shape = MX_ALLOC_N(size_t, src->dim);
    memcpy(dest->shape, src->shape, sizeof(size_t) * src->dim);

    size_t ptsize = dest->size * DTYPE_SIZES[dest->dtype];
    dest->elptr = MX_ALLOC_N(char, ptsize);
    memcpy(dest->elptr, src->elptr, ptsize);

    return dest;
}
