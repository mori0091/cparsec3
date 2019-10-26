/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T String
#define T_GENERATOR CONCAT(T, generator)

struct data {
  T a;
  T b;
  T c;
  T (*f)(T);
};

static T id(T x) {
  return x;
}

#define a ("a")
#define b ("b")
#define c ("c")
static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      {a, a, a, id}, {a, a, b, id}, {a, a, c, id}, //
      {a, b, a, id}, {a, b, b, id}, {a, b, c, id}, //
      {a, c, a, id}, {a, c, b, id}, {a, c, c, id}, //

      {b, a, a, id}, {b, a, b, id}, {b, a, c, id}, //
      {b, b, a, id}, {b, b, b, id}, {b, b, c, id}, //
      {b, c, a, id}, {b, c, b, id}, {b, c, c, id}, //

      {c, a, a, id}, {c, a, b, id}, {c, a, c, id}, //
      {c, b, a, id}, {c, b, b, id}, {c, b, c, id}, //
      {c, c, a, id}, {c, c, b, id}, {c, c, c, id}, //
  };
  if (i < sizeof(ret) / sizeof(ret[0])) {
    return &(ret[i]);
  }
  return NULL;
}
#undef a
#undef b
#undef c

#include "./test_eq.h"
