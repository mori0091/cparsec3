/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define EQ(T) FUNC_NAME(eq, Eq(T))
#define NEQ(T) FUNC_NAME(neq, Eq(T))

#define impl_Eq(T)                                                       \
  static bool EQ(T)(T a, T b) {                                          \
    return a == b;                                                       \
  }                                                                      \
  static bool NEQ(T)(T a, T b) {                                         \
    return a != b;                                                       \
  }                                                                      \
  Eq(T) Trait(Eq(T)) {                                                   \
    return (Eq(T)){.eq = EQ(T), .neq = NEQ(T)};                          \
  }                                                                      \
  END_OF_STATEMENTS

#define DISMISS_None JUST(1)
#define DISMISS_String JUST(1)
FOREACH(impl_Eq, FILTER_OUT_DISMISSED(TYPESET(ALL)));
#undef DISMISS_None
#undef DISMISS_String

static bool EQ(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return true;
}
instance_Eq(None, EQ(None));

static bool EQ(String)(String a, String b) {
  return !strcmp(a, b);
}
instance_Eq(String, EQ(String));

FOREACH(impl_Eq_Array, TYPESET_0);
FOREACH(impl_Eq_List, TYPESET_0);
FOREACH(impl_Eq_Maybe, TYPESET_0);

/* FOREACH(impl_Eq_Array, TYPESET_1); */
/* FOREACH(impl_Eq_List, TYPESET_1); */
/* FOREACH(impl_Eq_Maybe, TYPESET_1); */

FOREACH(impl_Eq_Maybe, TYPESET_ARRAY_1);
