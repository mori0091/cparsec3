/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T Maybe(int64_t)
#define T_GENERATOR FUNC_NAME(T, generator)

struct data {
  T a;
  T b;
  T c;
  T (*f)(T);
};

static T suc(T x) {
  if (x.none)
    return x;
  x.value++;
  return x;
}

#define a {.value = INT64_MIN}
#define b {.value = 0}
#define c {.value = INT64_MAX}
#define x {.none = true}
static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      // clang-format off
      {a, a, a, suc}, {a, a, b, suc}, {a, a, c, suc}, //
      {a, b, a, suc}, {a, b, b, suc}, {a, b, c, suc}, //
      {a, c, a, suc}, {a, c, b, suc}, {a, c, c, suc}, //

      {b, a, a, suc}, {b, a, b, suc}, {b, a, c, suc}, //
      {b, b, a, suc}, {b, b, b, suc}, {b, b, c, suc}, //
      {b, c, a, suc}, {b, c, b, suc}, {b, c, c, suc}, //

      {c, a, a, suc}, {c, a, b, suc}, {c, a, c, suc}, //
      {c, b, a, suc}, {c, b, b, suc}, {c, b, c, suc}, //
      {c, c, a, suc}, {c, c, b, suc}, {c, c, c, suc}, //
      // ----
      {x, x, x, suc},                                 //
      {x, x, a, suc}, {x, a, x, suc}, {x, a, a, suc}, //
      {a, x, x, suc}, {a, x, a, suc}, {a, a, x, suc}, //
      {x, a, b, suc}, {a, x, b, suc}, {a, b, x, suc}, //
      {x, b, a, suc}, {b, x, a, suc}, {b, a, x, suc}, //
      // clang-format on
  };
  if (i < sizeof(ret) / sizeof(ret[0])) {
    return &(ret[i]);
  }
  return NULL;
}
#undef a
#undef b
#undef c
#undef x

#include "../eq/test_eq.h"
