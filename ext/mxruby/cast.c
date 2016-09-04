#include <ruby.h>
#include "mx.h"

// DEFINE_CAST_METHOD(int8_t, int16_t, int8, int16);
// DEFINE_CAST_METHOD(int8_t, int32_t, int8, int32);
// DEFINE_CAST_METHOD(int8_t, int64_t, int8, int64);
// DEFINE_CAST_METHOD(int8_t, float,   int8, float32);
// DEFINE_CAST_METHOD(int8_t, double,  int8, float64);
//
// DEFINE_CAST_METHOD(int16_t, int8_t,  int16, int8);
// DEFINE_CAST_METHOD(int16_t, int32_t, int16, int32);
// DEFINE_CAST_METHOD(int16_t, int64_t, int16, int64);
// DEFINE_CAST_METHOD(int16_t, float,   int16, float32);
// DEFINE_CAST_METHOD(int16_t, double,  int16, float64);
//
// DEFINE_CAST_METHOD(int32_t, int8_t,  int32, int8);
// DEFINE_CAST_METHOD(int32_t, int16_t, int32, int16);
// DEFINE_CAST_METHOD(int32_t, int64_t, int32, int64);
// DEFINE_CAST_METHOD(int32_t, float,   int32, float32);
// DEFINE_CAST_METHOD(int32_t, double,  int32, float64);
//
// DEFINE_CAST_METHOD(int64_t, int8_t,  int64, int8);
// DEFINE_CAST_METHOD(int64_t, int16_t, int64, int16);
// DEFINE_CAST_METHOD(int64_t, int32_t, int64, int32);
// DEFINE_CAST_METHOD(int64_t, float,   int64, float32);
// DEFINE_CAST_METHOD(int64_t, double,  int64, float64);
//
// DEFINE_CAST_METHOD(float, int8_t,  float32, int8);
// DEFINE_CAST_METHOD(float, int16_t, float32, int16);
// DEFINE_CAST_METHOD(float, int32_t, float32, int32);
// DEFINE_CAST_METHOD(float, int64_t, float32, int64);
// DEFINE_CAST_METHOD(float, double,  float32, float64);
//
// DEFINE_CAST_METHOD(double, int8_t,  float64, int8);
// DEFINE_CAST_METHOD(double, int16_t, float64, int16);
// DEFINE_CAST_METHOD(double, int32_t, float64, int32);
// DEFINE_CAST_METHOD(double, int64_t, float64, int64);
// DEFINE_CAST_METHOD(double, float,   float64, float32);
//
// void mxx_cast(MX *mx, DTYPE new_dtype)
// {
//     size_t dsize = DTYPE_SIZES[new_dtype];
//     void *new_ptr = MX_ALLOC_N(char, mx->size * dsize);
//
//     switch (new_dtype) {
//         case DTYPE_INT8:
//             switch (mx->dtype) {
//                 case DTYPE_INT16:   mxx_cast_int8_from_int16(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT32:   mxx_cast_int8_from_int32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT64:   mxx_cast_int8_from_int64(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_int8_from_float32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_int8_from_float64(new_ptr, mx->elptr, mx->size); break;
//             }
//             break;
//         case DTYPE_INT16:
//             switch (mx->dtype) {
//                 case DTYPE_INT8:    mxx_cast_int16_from_int8(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT32:   mxx_cast_int16_from_int32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT64:   mxx_cast_int16_from_int64(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_int16_from_float32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_int16_from_float64(new_ptr, mx->elptr, mx->size); break;
//             }
//             break;
//         case DTYPE_INT32:
//             switch (mx->dtype) {
//                 case DTYPE_INT8:    mxx_cast_int32_from_int8(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT16:   mxx_cast_int32_from_int16(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT64:   mxx_cast_int32_from_int64(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_int32_from_float32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_int32_from_float64(new_ptr, mx->elptr, mx->size); break;
//             }
//             break;
//         case DTYPE_INT64:
//             switch (mx->dtype) {
//                 case DTYPE_INT8:    mxx_cast_int64_from_int8(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT16:   mxx_cast_int64_from_int16(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT32:   mxx_cast_int64_from_int32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_int64_from_float32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_int64_from_float64(new_ptr, mx->elptr, mx->size); break;
//             }
//             break;
//         case DTYPE_FLOAT32:
//             switch (mx->dtype) {
//                 case DTYPE_INT8:    mxx_cast_float32_from_int8(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT16:   mxx_cast_float32_from_int16(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT32:   mxx_cast_float32_from_int32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT64:   mxx_cast_float32_from_int64(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_float32_from_float64(new_ptr, mx->elptr, mx->size); break;
//             }
//             break;
//         case DTYPE_FLOAT64:
//             switch (mx->dtype) {
//                 case DTYPE_INT8:    mxx_cast_float64_from_int8(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT16:   mxx_cast_float64_from_int16(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT32:   mxx_cast_float64_from_int32(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_INT64:   mxx_cast_float64_from_int64(new_ptr, mx->elptr, mx->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_float64_from_float32(new_ptr, mx->elptr, mx->size); break;
//             }
//             break;
//     }
//
//     MX_FREE(mx->elptr);
//
//     mx->dtype = new_dtype;
//     mx->elptr = new_ptr;
// }
//
// MX *mxx_cast_copy(MX *src, DTYPE new_dtype)
// {
//     MX *dest = MX_ALLOC(MX);
//     dest->dim   = src->dim;
//     dest->size  = src-size;
//     dest->dtype = src->dtype;
//     dest->shape = MX_ALLOC_N(size_t, src->dim);
//     memcpy(dest->shape, src->shape, sizeof(size_t) * src->dim);
//
//     void *new_ptr = MX_ALLOC_N(char, dest->size * DTYPE_SIZES[new_dtype]);
//     switch (new_dtype) {
//         case DTYPE_INT8:
//             switch (src->dtype) {
//                 case DTYPE_INT16:   mxx_cast_int8_from_int16(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT32:   mxx_cast_int8_from_int32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT64:   mxx_cast_int8_from_int64(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_int8_from_float32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_int8_from_float64(new_ptr, src->elptr, src->size); break;
//             }
//             break;
//         case DTYPE_INT16:
//             switch (src->dtype) {
//                 case DTYPE_INT8:    mxx_cast_int16_from_int8(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT32:   mxx_cast_int16_from_int32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT64:   mxx_cast_int16_from_int64(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_int16_from_float32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_int16_from_float64(new_ptr, src->elptr, src->size); break;
//             }
//             break;
//         case DTYPE_INT32:
//             switch (src->dtype) {
//                 case DTYPE_INT8:    mxx_cast_int32_from_int8(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT16:   mxx_cast_int32_from_int16(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT64:   mxx_cast_int32_from_int64(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_int32_from_float32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_int32_from_float64(new_ptr, src->elptr, src->size); break;
//             }
//             break;
//         case DTYPE_INT64:
//             switch (src->dtype) {
//                 case DTYPE_INT8:    mxx_cast_int64_from_int8(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT16:   mxx_cast_int64_from_int16(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT32:   mxx_cast_int64_from_int32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_int64_from_float32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_int64_from_float64(new_ptr, src->elptr, src->size); break;
//             }
//             break;
//         case DTYPE_FLOAT32:
//             switch (src->dtype) {
//                 case DTYPE_INT8:    mxx_cast_float32_from_int8(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT16:   mxx_cast_float32_from_int16(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT32:   mxx_cast_float32_from_int32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT64:   mxx_cast_float32_from_int64(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT64: mxx_cast_float32_from_float64(new_ptr, src->elptr, src->size); break;
//             }
//             break;
//         case DTYPE_FLOAT64:
//             switch (src->dtype) {
//                 case DTYPE_INT8:    mxx_cast_float64_from_int8(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT16:   mxx_cast_float64_from_int16(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT32:   mxx_cast_float64_from_int32(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_INT64:   mxx_cast_float64_from_int64(new_ptr, src->elptr, src->size); break;
//                 case DTYPE_FLOAT32: mxx_cast_float64_from_float32(new_ptr, src->elptr, src->size); break;
//             }
//             break;
//     }
//
//     dest->elptr = new_ptr;
//     return dest;
// }
