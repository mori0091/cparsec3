/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define CMP(T) FUNC_NAME(cmp, Ord(T))
#define LT(T) FUNC_NAME(lt, Ord(T))
#define LE(T) FUNC_NAME(le, Ord(T))
#define GT(T) FUNC_NAME(gt, Ord(T))
#define GE(T) FUNC_NAME(ge, Ord(T))
#define MIN(T) FUNC_NAME(min, Ord(T))
#define MAX(T) FUNC_NAME(max, Ord(T))
#define EQ(T) trait(Eq(T)).eq

#define impl_Ord(T)                                                      \
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
  Ord(T) Trait(Ord(T)) {                                                 \
    return (Ord(T)){.cmp = CMP(T),                                       \
                    .lt = LT(T),                                         \
                    .le = LE(T),                                         \
                    .gt = GT(T),                                         \
                    .ge = GE(T),                                         \
                    .min = MIN(T),                                       \
                    .max = MAX(T)};                                      \
  }                                                                      \
  END_OF_STATEMENTS

#define DISMISS_None JUST(1)
#define DISMISS_String JUST(1)
FOREACH(impl_Ord, FILTER_OUT_DISMISSED(TYPESET(ALL)));
#undef DISMISS_None
#undef DISMISS_String

static int CMP(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return 0;
}
instance_Ord(None, CMP(None));

static int CMP(String)(String a, String b) {
  int x = strcmp(a, b);
  return (x <= 0 ? (x == 0 ? 0 : -1) : 1);
}
instance_Ord(String, CMP(String));
