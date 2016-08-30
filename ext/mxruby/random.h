#ifndef _MX_RANDOM_H_
#define _MX_RANDOM_H_

#include <stdbool.h>

#define NUM_STATE 16
#define MIN_DOUBLE 5.421010862427522e-20

typedef struct _MXRANDOM {
    uint64_t s[NUM_STATE];
    int p;
    bool has_value;
    double value;
} MXRANDOM;

#endif // _MX_RANDOM_H_
