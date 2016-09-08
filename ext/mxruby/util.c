#include <ruby.h>
#include "mx.h"

MX *mxx_copy_shape(MX *src)
{
    MX *dest = MX_ALLOC(MX);
    dest->dim   = src->dim;
    dest->size  = src->size;
    dest->shape = MX_ALLOC_N(size_t, src->dim);
    memcpy(dest->shape, src->shape, sizeof(size_t) * src->dim);
    return dest;
}

DTYPE mxx_dtype_from_symbol(VALUE dtype)
{
    DTYPE ret = DEFAULT_DTYPE;
    for (int i = 0; i < NUM_DTYPE; i++) {
        if (SYM2ID(dtype) == rb_intern(DTYPE_NAMES[i])) {
            return i;
        }
    }

    return ret;
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
