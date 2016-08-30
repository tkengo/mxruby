#ifndef _MX_RANDOM_H_
#define _MX_RANDOM_H_

#define NUM_STATE 16
#define MIN_DOUBLE 5.421010862427522170037264004350e-20

typedef struct _MXRANDOM {
    uint64_t s[NUM_STATE];
    int p;
} MXRANDOM;

#endif // _MX_RANDOM_H_
