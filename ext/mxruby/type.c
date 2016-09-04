#include <ruby.h>
#include "mx.h"

DTYPE mxx_dtype_from_symbol(VALUE dtype)
{
    DTYPE ret = DEFAULT_DTYPE;
    for (int i = 0; i < NUM_DTYPE; i++) {
        if (SYM2ID(dtype) == rb_intern(DTYPE_NAMES[i])) {
            return ret = i;
        }
    }

    return ret;
}
