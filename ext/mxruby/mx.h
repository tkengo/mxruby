#ifndef _MX_MX_H_
#define _MX_MX_H_

#include <float.h>
#include <stdbool.h>

#define UINT64_MIN 5.421010862427522e-20
#define DBL_IS_INT(dbl) (dbl - floor(dbl) <= DBL_MIN)
#define MX_MAX(a,b) ((a) > (b) ? (a) : (b))

#define NUM_DTYPE 6

#define DTYPE_INT8    0
#define DTYPE_INT16   1
#define DTYPE_INT32   2
#define DTYPE_INT64   3
#define DTYPE_FLOAT32 4
#define DTYPE_FLOAT64 5
#define DEFAULT_DTYPE DTYPE_FLOAT64

#define DTYPE_IS_INT(dtype) ((dtype) == DTYPE_INT8 || \
        (dtype) == DTYPE_INT16 || \
        (dtype) == DTYPE_INT32 || \
        (dtype) == DTYPE_INT64)
#define UP_DTYPE(d1,d2) (d1 > d2 ? d1 : d2)

#define MX_ALLOC(type) (ALLOC(type))
#define MX_ALLOC_N(type,n) (ALLOC_N(type,n))
#define MX_FREE(var) (xfree(var))

#define MX_RAND_NUM_STATE 16

#define MX_DATA_PTR(mx) ((MX *)DATA_PTR(mx))
#define MX_DATA_ELPTR(mx) (MX_DATA_PTR(mx)->elptr)
#define MX_DATA_DIM(mx) (MX_DATA_PTR(mx)->dim)
#define MX_DATA_SIZE(mx) (MX_DATA_PTR(mx)->size)

#define MX_INIT(shape) mxx_initialize(shape, DEFAULT_DTYPE)
#define MX_INIT_D(shape, dtype) mxx_initialize(shape, dtype)
#define IS_MX(v) (CLASS_OF(v) == rb_cMx)

typedef short DTYPE;

typedef struct _MX {
    size_t dim;
    size_t size;
    size_t *shape;
    DTYPE dtype;
    void *elptr;
} MX;

typedef struct _MXRANDOM {
    uint64_t s[MX_RAND_NUM_STATE];
    int p;
    bool has_value;
    double value;
} MXRANDOM;

static const char * DTYPE_NAMES[NUM_DTYPE] = {
    "int8",
    "int16",
    "int32",
    "int64",
    "float32",
    "float64"
};

static const size_t DTYPE_SIZES[NUM_DTYPE] = {
    sizeof(int8_t),
    sizeof(int16_t),
    sizeof(int32_t),
    sizeof(int64_t),
    sizeof(float),   // as float32
    sizeof(double)   // as float64
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern VALUE rb_cMx;
extern VALUE rb_eDataTypeError;
extern void Init_random();

extern void mxx_cast(MX *mx, DTYPE new_dtype);
extern MX *mxx_cast_copy(MX *src, DTYPE new_dtype);

extern void mxx_free(MX *mx);
extern MX *mxx_initialize(VALUE shape, DTYPE dtype);
extern void mxx_setup(MX *mx, DTYPE dtype);
extern void mxx_initialize_shape(MX *mx, VALUE shape);
extern DTYPE mxx_dtype_from_symbol(VALUE dtype);

/**
 * Defined in util.c
 */
extern MX *mxx_copy_shape(MX *src);
extern MX *mxx_cast_copy(MX *src, DTYPE dtype);
extern bool mxx_is_same_shape(MX *m1, MX *m2);

/**
 * Defined in ewop.cpp
 */
extern MX *mxx_ewmul_array(MX *l, MX *r);
extern void mxx_ewmul_scalar(MX *l, double r);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif // _MX_MX_H_
