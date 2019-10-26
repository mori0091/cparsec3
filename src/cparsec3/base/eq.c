/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define EQ(T) CONCAT(eq, T)
#define NEQ(T) CONCAT(neq, T)

#define define_Eq(T)                                                     \
  static bool EQ(T)(T a, T b) {                                          \
    return a == b;                                                       \
  }                                                                      \
  static bool NEQ(T)(T a, T b) {                                         \
    return a != b;                                                       \
  }                                                                      \
  Eq(T) CPARSEC_MODULE(Eq(T))(void) {                                    \
    return (Eq(T)){.eq = EQ(T), .neq = NEQ(T)};                          \
  }                                                                      \
  END_OF_STATEMENTS

define_Eq(char);
define_Eq(int);

define_Eq(int8_t);
define_Eq(int16_t);
define_Eq(int32_t);
define_Eq(int64_t);

define_Eq(uint8_t);
define_Eq(uint16_t);
define_Eq(uint32_t);
define_Eq(uint64_t);

static bool EQ(String)(String a, String b) {
  return !strcmp(a, b);
}
static bool NEQ(String)(String a, String b) {
  return !!strcmp(a, b);
}
Eq(String) CPARSEC_MODULE(Eq(String))(void) {
  return (Eq(String)){.eq = EQ(String), .neq = NEQ(String)};
}
