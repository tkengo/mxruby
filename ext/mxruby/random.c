#include <ruby.h>
#include "mx.h"

static void mx_random_free(MXRANDOM *r)
{
    MX_FREE(mx);
}

static VALUE mx_random_alloc(VALUE klass)
{
    MXRANDOM *r = ALLOC(MXRANDOM);

    return Data_Wrap_Struct(klass, 0, mx_random_free, r);
}

static VALUE mx_random_initialize(VALUE self, VALUE seed)
{
}

static VALUE mx_random_rand(VALUE self) {
    MXRANDOM *r;
    Data_Get_Struct(self, MXRANDOM, r);

    const uint64_t s0 = s[p];
    uint64_t s1 = s[p = (p + 1) & 15];
    s1 ^= s1 << 31;
    s[p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30);
    return s[p] * UINT64_C(1181783497276652981);
}

static VALUE mx_random_randn(VALUE self)
{
    return INT2FIX(2);
}

void Init_random()
{
    rb_cRandom = rb_define_class_under(rb_cMx, "Random", rb_cObject);

    rb_define_alloc_func(rb_cRandom, mx_random_alloc);

    rb_define_method(rb_cRandom, "initialize", mx_random_initialize, 1);

    rb_define_singleton_method(rb_cRandom, "rand", mx_random_rand, 0);
    rb_define_singleton_method(rb_cRandom, "randn", mx_random_randn, 0);
}
