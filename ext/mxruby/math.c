#include "mx.h"

double int_pow(double a, long b) {
    if (a == 0) {
        return 0;
    }
    if (a == 1 || b == 0) {
        return 1;
    }

    double result = 1;
    while (b > 0) {
        if (b & 1) {
            result *= a;
        }
        b >>= 1;
        a *= a;
    }

    return result;
}
