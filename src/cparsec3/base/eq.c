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

#define DISMISS_None JUST(1)
#define DISMISS_String JUST(1)
FOREACH(define_Eq, FILTER_OUT_DISMISSED(TYPESET(ALL)));
#undef DISMISS_None
#undef DISMISS_String

static bool EQ(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return true;
}
static bool NEQ(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return false;
}
Eq(None) CPARSEC_MODULE(Eq(None))(void) {
  return (Eq(None)){.eq = EQ(None), .neq = NEQ(None)};
}

static bool EQ(String)(String a, String b) {
  return !strcmp(a, b);
}
static bool NEQ(String)(String a, String b) {
  return !!strcmp(a, b);
}
Eq(String) CPARSEC_MODULE(Eq(String))(void) {
  return (Eq(String)){.eq = EQ(String), .neq = NEQ(String)};
}
