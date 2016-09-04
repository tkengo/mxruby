#ifndef _MX_EWOP_H_
#define _MX_EWOP_H_

#include <ruby.h>

template <typename L, typename R>
void *mxt_ewmul(L *l, R* r, size_t n) {
    size_t lsize = sizeof(L);
    size_t rsize = sizeof(R);
    size_t dsize = lsize > rsize ? lsize : rsize;
    void *new_ptr = MX_ALLOC_N(char, n * dsize);

    if (lsize > rsize) {
        for (size_t i = 0; i < n; i++) {
            *(((L *)new_ptr) + i) = *(((L *)l) + i) * *(((R *)r) + i);
        }
    } else {
        for (size_t i = 0; i < n; i++) {
            *(((R *)new_ptr) + i) = *(((L *)l) + i) * *(((R *)r) + i);
        }
    }

    return new_ptr;
}

#endif // _MX_EWOP_H_
