/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T None
#define T_GENERATOR FUNC_NAME(T, generator)

struct data {
  T a;
  T b;
  T c;
  T (*f)(T);
};

static T suc(T x) {
  return x;
}

static void* T_GENERATOR(size_t i) {
  static struct data ret = { {0}, {0}, {0}, suc };
  if (i < 1) {
    return &ret;
  }
  return NULL;
}

#include "./test_eq.h"
