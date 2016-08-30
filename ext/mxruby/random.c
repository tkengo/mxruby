#include <ruby.h>
#include <time.h>
#include "mx.h"
#include "random.h"

static void mx_random_free(MXRANDOM *r)
{
    MX_FREE(r);
}

static VALUE mx_random_alloc(VALUE klass)
{
    MXRANDOM *r = MX_ALLOC(MXRANDOM);

    return Data_Wrap_Struct(klass, 0, mx_random_free, r);
}

static VALUE mx_random_initialize(int argc, VALUE *argv, VALUE self)
{
    time_t t;
    VALUE rb_seed;
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);

    rb_scan_args(argc, argv, "01", &rb_seed);
    srand(NIL_P(rb_seed) ? time(&t) : FIX2INT(rb_seed));

    r->p = 0;
    for (int i = 0; i < NUM_STATE; i++) {
        r->s[i] = rand();
    }

    return self;
}

static uint64_t _xorshift_star_1024(MXRANDOM *r)
{
    const uint64_t s0 = r->s[r->p];
    uint64_t s1 = r->s[r->p = (r->p + 1) & 15];
    s1 ^= s1 << 31;
    r->s[r->p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30);
    return r->s[r->p] * UINT64_C(1181783497276652981);
}

static VALUE mx_random_rand(VALUE self) {
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);

    double v = _xorshift_star_1024(r) * MIN_DOUBLE;
    return rb_float_new(v);
}

static VALUE mx_random_randn(VALUE self)
{
    return INT2FIX(2);
}

void Init_random()
{
    rb_cRandom = rb_define_class_under(rb_cMx, "Random", rb_cObject);

    rb_define_alloc_func(rb_cRandom, mx_random_alloc);

    rb_define_method(rb_cRandom, "initialize", mx_random_initialize, -1);
    rb_define_method(rb_cRandom, "rand", mx_random_rand, 0);
    rb_define_method(rb_cRandom, "randn", mx_random_randn, 0);
}
