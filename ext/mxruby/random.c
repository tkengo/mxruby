#include <ruby.h>
#include <time.h>
#include "mx.h"

void mxx_random_free(MXRANDOM *r)
{
    MX_FREE(r);
}

double mxx_random_xorshift1024(MXRANDOM *r)
{
    const uint64_t s0 = r->s[r->p];
    uint64_t s1 = r->s[r->p = (r->p + 1) & 15];
    s1 ^= s1 << 31;
    r->s[r->p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30);
    return r->s[r->p] * UINT64_C(1181783497276652981) * UINT64_MIN;
}

double mxx_random_gaussian(MXRANDOM *r)
{
    if (r->has_value) {
        r->has_value = false;
        return r->value;
    } else {
        double x1, x2, s;
        do {
            x1 = 2.0 * mxx_random_xorshift1024(r) - 1.0;
            x2 = 2.0 * mxx_random_xorshift1024(r) - 1.0;
            s = (x1 * x1) + (x2 * x2);
        } while (s == 0 || s >= 1);

        double p = sqrt(-2 * log(s) / s);
        double z1 = x1 * p;
        double z2 = x2 * p;

        r->has_value = true;
        r->value = z2;
        return z1;
    }
}

static VALUE mx_random_alloc(VALUE klass)
{
    MXRANDOM *r = MX_ALLOC(MXRANDOM);

    return Data_Wrap_Struct(klass, 0, mxx_random_free, r);
}

static VALUE mx_random_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_seed;
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);

    rb_scan_args(argc, argv, "01", &rb_seed);
    srand(NIL_P(rb_seed) ? time(NULL) : FIX2INT(rb_seed));

    r->has_value = false;
    r->p = 0;
    for (int i = 0; i < MX_RAND_NUM_STATE; i++) {
        r->s[i] = rand();
    }

    return self;
}

static VALUE mx_random_rand(int argc, VALUE *argv, VALUE self) {
    VALUE shape;
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);
    rb_scan_args(argc, argv, "01", &shape);

    if (NIL_P(shape)) {
        return rb_float_new(mxx_random_xorshift1024(r));
    }

    MX *mx = MX_INIT_D(shape, DTYPE_FLOAT64);
    for (int i = 0; i < mx->size; i++) {
        *(double *)(mx->elptr + i * DTYPE_SIZES[mx->dtype]) = mxx_random_xorshift1024(r);
    }

    return Data_Wrap_Struct(rb_cMx, 0, mxx_free, mx);
}

static VALUE mx_random_randn(int argc, VALUE *argv, VALUE self)
{
    VALUE shape;
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);
    rb_scan_args(argc, argv, "01", &shape);

    if (NIL_P(shape)) {
        return rb_float_new(mxx_random_gaussian(r));
    }

    MX *mx = MX_INIT_D(shape, DTYPE_FLOAT64);
    for (int i = 0; i < mx->size; i++) {
        *(double *)(mx->elptr + i * DTYPE_SIZES[mx->dtype]) = mxx_random_gaussian(r);
    }

    return Data_Wrap_Struct(rb_cMx, 0, mxx_free, mx);
}

void Init_random()
{
    rb_cRandom = rb_define_class_under(rb_cMx, "Random", rb_cObject);

    rb_define_alloc_func(rb_cRandom, mx_random_alloc);

    rb_define_method(rb_cRandom, "initialize", mx_random_initialize, -1);
    rb_define_method(rb_cRandom, "rand", mx_random_rand, -1);
    rb_define_method(rb_cRandom, "randn", mx_random_randn, -1);
}
