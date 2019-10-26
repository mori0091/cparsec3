/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define CMP(T) CONCAT(cmp, T)
#define LT(T) CONCAT(lt, T)
#define LE(T) CONCAT(le, T)
#define GT(T) CONCAT(gt, T)
#define GE(T) CONCAT(ge, T)
#define MIN(T) CONCAT(MIN, T)
#define MAX(T) CONCAT(MAX, T)
#define EQ(T) cparsec_module(Eq(T)).eq

#define define_Ord(T)                                                    \
  static bool LT(T)(T a, T b) {                                          \
    return a < b;                                                        \
  }                                                                      \
  static bool LE(T)(T a, T b) {                                          \
    return a <= b;                                                       \
  }                                                                      \
  static bool GT(T)(T a, T b) {                                          \
    return a > b;                                                        \
  }                                                                      \
  static bool GE(T)(T a, T b) {                                          \
    return a >= b;                                                       \
  }                                                                      \
  static int CMP(T)(T a, T b) {                                          \
    return (LE(T)(a, b) ? (EQ(T)(a, b) ? 0 : -1) : 1);                   \
  }                                                                      \
  static T MIN(T)(T a, T b) {                                            \
    return LE(T)(a, b) ? a : b;                                          \
  }                                                                      \
  static T MAX(T)(T a, T b) {                                            \
    return LE(T)(b, a) ? a : b;                                          \
  }                                                                      \
  Ord(T) CPARSEC_MODULE(Ord(T))(void) {                                  \
    return (Ord(T)){.cmp = CMP(T),                                       \
                    .lt = LT(T),                                         \
                    .le = LE(T),                                         \
                    .gt = GT(T),                                         \
                    .ge = GE(T),                                         \
                    .min = MIN(T),                                       \
                    .max = MAX(T)};                                      \
  }                                                                      \
  END_OF_STATEMENTS

define_Ord(char);
define_Ord(int);

define_Ord(int8_t);
define_Ord(int16_t);
define_Ord(int32_t);
define_Ord(int64_t);

define_Ord(uint8_t);
define_Ord(uint16_t);
define_Ord(uint32_t);
define_Ord(uint64_t);

static int CMP(String)(String a, String b) {
  return strcmp(a, b);
}
static bool LT(String)(String a, String b) {
  return CMP(String)(a, b) < 0;
}
static bool LE(String)(String a, String b) {
  return CMP(String)(a, b) <= 0;
}
static bool GT(String)(String a, String b) {
  return CMP(String)(a, b) > 0;
}
static bool GE(String)(String a, String b) {
  return CMP(String)(a, b) >= 0;
}
static String MIN(String)(String a, String b) {
  return LE(String)(a, b) ? a : b;
}
static String MAX(String)(String a, String b) {
  return LE(String)(b, a) ? a : b;
}
Ord(String) CPARSEC_MODULE(Ord(String))(void) {
  return (Ord(String)){.cmp = CMP(String),
                       .lt = LT(String),
                       .le = LE(String),
                       .gt = GT(String),
                       .ge = GE(String),
                       .min = MIN(String),
                       .max = MAX(String)};
}
