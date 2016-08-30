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
    VALUE rb_seed;
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);

    rb_scan_args(argc, argv, "01", &rb_seed);
    srand(NIL_P(rb_seed) ? time(NULL) : FIX2INT(rb_seed));

    r->has_value = false;
    r->p = 0;
    for (int i = 0; i < NUM_STATE; i++) {
        r->s[i] = rand();
    }

    return self;
}

static double _xorshift_star_1024(MXRANDOM *r)
{
    const uint64_t s0 = r->s[r->p];
    uint64_t s1 = r->s[r->p = (r->p + 1) & 15];
    s1 ^= s1 << 31;
    r->s[r->p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30);
    return r->s[r->p] * UINT64_C(1181783497276652981) * MIN_DOUBLE;
}

static VALUE mx_random_rand(VALUE self) {
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);

    return rb_float_new(_xorshift_star_1024(r));
}

static VALUE mx_random_randn(VALUE self)
{
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);

    double v;
    if (r->has_value) {
        v = r->value;
        r->has_value = false;
    } else {
        double x1 = _xorshift_star_1024(r);
        double x2 = _xorshift_star_1024(r);

        double s = (x1 * x1) + (x2 * x2);
        double p = sqrt(-2 * log(s) / s);
        double z1 = x1 * p;
        double z2 = x2 * p;

        v = z1;
        r->has_value = true;
        r->value = z2;
    }

    return rb_float_new(v);
}

void Init_random()
{
    rb_cRandom = rb_define_class_under(rb_cMx, "Random", rb_cObject);

    rb_define_alloc_func(rb_cRandom, mx_random_alloc);

    rb_define_method(rb_cRandom, "initialize", mx_random_initialize, -1);
    rb_define_method(rb_cRandom, "rand", mx_random_rand, 0);
    rb_define_method(rb_cRandom, "randn", mx_random_randn, 0);
}
