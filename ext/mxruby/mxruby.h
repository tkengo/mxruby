#define MX_ALLOC(type) (ALLOC(type))
#define MX_ALLOC_N(type,n) (ALLOC_N(type,n))
#define MX_FREE(var) (xfree(var))

typedef char DTYPE;

typedef struct _MX {
    size_t dim;
    size_t size;
    size_t *shape;
    DTYPE dtype;
    void *elptr;
} MX;

#define NUM_DTYPE 6

#define DTYPE_INT8    0
#define DTYPE_INT16   1
#define DTYPE_INT32   2
#define DTYPE_INT64   3
#define DTYPE_FLOAT32 4
#define DTYPE_FLOAT64 5

const char * DTYPE_NAMES[NUM_DTYPE] = {
    "int8",
    "int16",
    "int32",
    "int64",
    "float32",
    "float64"
};

const size_t DTYPE_SIZES[NUM_DTYPE] = {
    sizeof(int8_t),
    sizeof(int16_t),
    sizeof(int32_t),
    sizeof(int64_t),
    sizeof(float),   // as float32
    sizeof(double)   // as float64
};

#define MX_DATA_PTR(mx) ((MX *)DATA_PTR(mx))
#define MX_DATA_ELPTR(mx) (MX_DATA_PTR(mx)->elptr)
#define MX_DATA_DIM(mx) (MX_DATA_PTR(mx)->dim)
#define MX_DATA_SIZE(mx) (MX_DATA_PTR(mx)->size)
