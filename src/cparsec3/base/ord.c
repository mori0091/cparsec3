/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define CMP(T) CONCAT(cmp, T)
#define LT(T) CONCAT(lt, T)
#define LE(T) CONCAT(le, T)
#define GT(T) CONCAT(gt, T)
#define GE(T) CONCAT(ge, T)
#define MIN(T) CONCAT(min, T)
#define MAX(T) CONCAT(max, T)
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

#define DISMISS_None JUST(1)
#define DISMISS_String JUST(1)
FOREACH(define_Ord, FILTER_OUT_DISMISSED(TYPESET(ALL)));
#undef DISMISS_None
#undef DISMISS_String

static int CMP(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return 0;
}
static bool LT(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return false;
}
static bool LE(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return true;
}
static bool GT(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return false;
}
static bool GE(None)(None a, None b) {
  UNUSED(a);
  UNUSED(b);
  return true;
}
static None MIN(None)(None a, None b) {
  UNUSED(b);
  return a;
}
static None MAX(None)(None a, None b) {
  UNUSED(b);
  return a;
}
Ord(None) CPARSEC_MODULE(Ord(None))(void) {
  return (Ord(None)){.cmp = CMP(None),
                     .lt = LT(None),
                     .le = LE(None),
                     .gt = GT(None),
                     .ge = GE(None),
                     .min = MIN(None),
                     .max = MAX(None)};
}

static int CMP(String)(String a, String b) {
  int x = strcmp(a, b);
  return (x <= 0 ? (x == 0 ? 0 : -1) : 1);
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
