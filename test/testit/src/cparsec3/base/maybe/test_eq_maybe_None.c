/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T Maybe(None)
#define T_GENERATOR CONCAT(T, generator)

struct data {
  T a;
  T b;
  T c;
  T (*f)(T);
};

static T suc(T x) {
  x.none = !x.none;
  return x;
}

#define a {.value = {0}}
#define x {.none = true}
static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      // clang-format off
      {x, x, x, suc},                                 //
      {x, x, a, suc}, {x, a, x, suc}, {x, a, a, suc}, //
      {a, x, x, suc}, {a, x, a, suc}, {a, a, x, suc}, //
      // clang-format on
  };
  if (i < sizeof(ret) / sizeof(ret[0])) {
    return &(ret[i]);
  }
  return NULL;
}
#undef a
#undef x

#include "../eq/test_eq.h"
