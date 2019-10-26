/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T char
#define T_GENERATOR CONCAT(T, generator)

struct data {
  T a;
  T b;
  T c;
  T (*f)(T);
};

static T suc(T x) {
  return x + 1;
}

#define a ('a')
#define b ('b')
#define c ('c')
static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      {a, a, a, suc}, {a, a, b, suc}, {a, a, c, suc}, //
      {a, b, a, suc}, {a, b, b, suc}, {a, b, c, suc}, //
      {a, c, a, suc}, {a, c, b, suc}, {a, c, c, suc}, //

      {b, a, a, suc}, {b, a, b, suc}, {b, a, c, suc}, //
      {b, b, a, suc}, {b, b, b, suc}, {b, b, c, suc}, //
      {b, c, a, suc}, {b, c, b, suc}, {b, c, c, suc}, //

      {c, a, a, suc}, {c, a, b, suc}, {c, a, c, suc}, //
      {c, b, a, suc}, {c, b, b, suc}, {c, b, c, suc}, //
      {c, c, a, suc}, {c, c, b, suc}, {c, c, c, suc}, //
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
