/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define T None
#define T_GENERATOR CONCAT(T, generator)

struct data {
  T a;
  T b;
  T c;
};

static void* T_GENERATOR(size_t i) {
  static struct data ret = {{0}, {0}, {0}};
  if (i < 1) {
    return &ret;
  }
  return NULL;
}

#include "./test_ord.h"
