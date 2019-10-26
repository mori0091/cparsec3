/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T uint16_t
#define T_GENERATOR CONCAT(T, generator)

struct data {
  T a;
  T b;
  T c;
};

#define a (0)
#define b (1)
#define c (UINT16_MAX)
static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      {a, a, a}, {a, a, b}, {a, a, c}, //
      {a, b, a}, {a, b, b}, {a, b, c}, //
      {a, c, a}, {a, c, b}, {a, c, c}, //

      {b, a, a}, {b, a, b}, {b, a, c}, //
      {b, b, a}, {b, b, b}, {b, b, c}, //
      {b, c, a}, {b, c, b}, {b, c, c}, //

      {c, a, a}, {c, a, b}, {c, a, c}, //
      {c, b, a}, {c, b, b}, {c, b, c}, //
      {c, c, a}, {c, c, b}, {c, c, c}, //
  };
  if (i < sizeof(ret) / sizeof(ret[0])) {
    return &(ret[i]);
  }
  return NULL;
}
#undef a
#undef b
#undef c

#include "./test_ord.h"
