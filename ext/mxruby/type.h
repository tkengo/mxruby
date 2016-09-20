#ifndef _MX_EWOP_H_
#define _MX_EWOP_H_

#include <ruby.h>

template <typename L, typename R>
void *mxt_ewadd_array(L *l, R* r, size_t n)
{
    size_t lsize = sizeof(L);
    size_t rsize = sizeof(R);
    size_t dsize = lsize > rsize ? lsize : rsize;
    void *new_ptr = MX_ALLOC_N(char, n * dsize);

    if (lsize > rsize) {
        for (size_t i = 0; i < n; i++) {
            *(((L *)new_ptr) + i) = *(((L *)l) + i) + (L)(*(((R *)r) + i));
        }
    } else {
        for (size_t i = 0; i < n; i++) {
            *(((R *)new_ptr) + i) = (R)(*(((L *)l) + i)) + *(((R *)r) + i);
        }
    }

    return new_ptr;
}

template <typename L, typename R>
void *mxt_ewsub_array(L *l, R* r, size_t n)
{
    size_t lsize = sizeof(L);
    size_t rsize = sizeof(R);
    size_t dsize = lsize > rsize ? lsize : rsize;
    void *new_ptr = MX_ALLOC_N(char, n * dsize);

    if (lsize > rsize) {
        for (size_t i = 0; i < n; i++) {
            *(((L *)new_ptr) + i) = *(((L *)l) + i) - (L)(*(((R *)r) + i));
        }
    } else {
        for (size_t i = 0; i < n; i++) {
            *(((R *)new_ptr) + i) = (R)(*(((L *)l) + i)) - *(((R *)r) + i);
        }
    }

    return new_ptr;
}

template <typename L, typename R>
void *mxt_ewmul_array(L *l, R* r, size_t n)
{
    size_t lsize = sizeof(L);
    size_t rsize = sizeof(R);
    size_t dsize = lsize > rsize ? lsize : rsize;
    void *new_ptr = MX_ALLOC_N(char, n * dsize);

    if (lsize > rsize) {
        for (size_t i = 0; i < n; i++) {
            *(((L *)new_ptr) + i) = *(((L *)l) + i) * (L)(*(((R *)r) + i));
        }
    } else {
        for (size_t i = 0; i < n; i++) {
            *(((R *)new_ptr) + i) = (R)(*(((L *)l) + i)) * *(((R *)r) + i);
        }
    }

    return new_ptr;
}

template <typename L, typename R>
void *mxt_ewpow_array(L *l, R* r, size_t n)
{
    void *new_ptr = MX_ALLOC_N(char, n * sizeof(double));
    for (size_t i = 0; i < n; i++) {
        *(((double *)new_ptr) + i) = pow(*(((L *)l) + i), *(((R *)r) + i));
    }

    return new_ptr;
}

template <typename L, typename R>
void *mxt_ewintpow_array(L *l, R* r, size_t n)
{
    void *new_ptr = MX_ALLOC_N(char, n * sizeof(L));
    for (size_t i = 0; i < n; i++) {
        *(((L *)new_ptr) + i) = (L)int_pow(*(((L *)l) + i), (int)*(((R *)r) + i));
    }

    return new_ptr;
}

template <typename L, typename N>
void *mxt_ewadd_scalar(L *l, N r, size_t n)
{
    void *new_ptr = MX_ALLOC_N(N, n);
    for (size_t i = 0; i < n; i++) {
        *(((N *)new_ptr) + i) = *(((L *)l) + i) + r;
    }
    return new_ptr;
}

template <typename L, typename N>
void *mxt_ewsub_scalar(L *l, N r, size_t n)
{
    void *new_ptr = MX_ALLOC_N(N, n);
    for (size_t i = 0; i < n; i++) {
        *(((N *)new_ptr) + i) = *(((L *)l) + i) - r;
    }
    return new_ptr;
}

template <typename L, typename N>
void *mxt_ewmul_scalar(L *l, N r, size_t n)
{
    void *new_ptr = MX_ALLOC_N(N, n);
    for (size_t i = 0; i < n; i++) {
        *(((N *)new_ptr) + i) = *(((L *)l) + i) * r;
    }
    return new_ptr;
}

template <typename L>
void *mxt_ewpow_intscalar(L *l, L r, size_t n)
{
    void *new_ptr = MX_ALLOC_N(L, n);
    for (size_t i = 0; i < n; i++) {
        *(((L *)new_ptr) + i) = (L)int_pow(*(((L *)l) + i), r);
    }
    return new_ptr;
}

template <typename L>
void *mxt_ewpow_dblscalar(L *l, double r, size_t n)
{
    void *new_ptr = MX_ALLOC_N(double, n);
    for (size_t i = 0; i < n; i++) {
        *(((double *)new_ptr) + i) = pow(*(((L *)l) + i), r);
    }
    return new_ptr;
}

template <typename T>
void mxt_copy(T *src, T *dest, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        *(dest + i) = *(src + i);
    }
}

template <typename D, typename S>
void mxt_cast(D *d, S* s, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        *(((D *)d) + i) = (D)(*(((S *)s) + i));
    }
}

template <typename T>
double mxt_sum(T *p, size_t n)
{
    double sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += *(((T *)p) + i);
    }
    return sum;
}

template <typename T>
void mxt_linspace(T *p, size_t n, T start, T step)
{
    T current = start;
    for (size_t i = 0; i < n; i++) {
        *(((T *)p) + i) = current;
        current += step;
    }
}

template <typename T>
void mxt_eye(T *p, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        *(((T *)p) + (i * n + i)) = 1;
    }
}

#endif // _MX_EWOP_H_
