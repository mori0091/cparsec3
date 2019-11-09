/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T Maybe(int8_t)
#define T_GENERATOR FUNC_NAME(T, generator)

struct data {
  T a;
  T b;
  T c;
};

#define a                                                                \
  { .value = INT8_MIN }
#define b                                                                \
  { .value = 0 }
#define c                                                                \
  { .value = INT8_MAX }
#define x                                                                \
  { .none = true }
static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      // clang-format off
      {a, a, a}, {a, a, b}, {a, a, c}, //
      {a, b, a}, {a, b, b}, {a, b, c}, //
      {a, c, a}, {a, c, b}, {a, c, c}, //

      {b, a, a}, {b, a, b}, {b, a, c}, //
      {b, b, a}, {b, b, b}, {b, b, c}, //
      {b, c, a}, {b, c, b}, {b, c, c}, //

      {c, a, a}, {c, a, b}, {c, a, c}, //
      {c, b, a}, {c, b, b}, {c, b, c}, //
      {c, c, a}, {c, c, b}, {c, c, c}, //
      // ----
      {x, x, x},                       //
      {x, x, a}, {x, a, x}, {x, a, a}, //
      {a, x, x}, {a, x, a}, {a, a, x}, //
      {x, a, b}, {a, x, b}, {a, b, x}, //
      {x, b, a}, {b, x, a}, {b, a, x}, //
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

#include "../ord/test_ord.h"
