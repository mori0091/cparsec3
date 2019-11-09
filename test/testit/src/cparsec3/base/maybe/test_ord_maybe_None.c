/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T Maybe(None)
#define T_GENERATOR FUNC_NAME(T, generator)

struct data {
  T a;
  T b;
  T c;
};

#define a                                                                \
  {                                                                      \
    .value = { 0 }                                                       \
  }
#define x                                                                \
  { .none = true }
static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      // clang-format off
      {x, x, x},                       //
      {x, x, a}, {x, a, x}, {x, a, a}, //
      {a, x, x}, {a, x, a}, {a, a, x}, //
      // clang-format on
  };
  if (i < sizeof(ret) / sizeof(ret[0])) {
    return &(ret[i]);
  }
  return NULL;
}
#undef a
#undef x

#include "../ord/test_ord.h"
