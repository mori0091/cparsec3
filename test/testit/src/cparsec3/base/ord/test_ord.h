/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#define _DISABLE_TESTIT_OPERATORS
#define TestSuite Ord(T)
#include "testit.h"

#define E cparsec_module(Eq(T))
#define O cparsec_module(Ord(T))

#define EQ E.eq   /* == E.equal */
#define NEQ E.neq /* == E.notEqual */

#define LE O.le   /* == O.lessThanEqual */
#define LT O.lt   /* == O.lessThan */
#define GE O.ge   /* == O.greaterThanEqual */
#define GT O.gt   /* == O.greaterThan */
#define CMP O.cmp /* == O.compare */

#define MIN O.min
#define MAX O.max

#define NA true

test("Transitivity: if (a <= b && b <= c) := true, then (a <= c) := true",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  T c = x->c;
  c_assert((LE(a, b) && LE(b, c)) ? LE(a, c) : NA);
}

test("Reflexivity: (a <= a) := true", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  c_assert(LE(a, a));
}

test("Antisymmetry: if (a <= b && b <= a) := true, then (a == b) := true",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert((LE(a, b) && LE(b, a)) ? EQ(a, b) : NA);
}

test("(1) (a >= b) := (b <= a)", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(GE(a, b) == LE(b, a));
}

test("(2) (a < b) := (a <= b && a != b)", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(LT(a, b) == (LE(a, b) && NEQ(a, b)));
}

test("(3) (a > b) := (b < a)", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(GT(a, b) == LT(b, a));
}

test("(4) (a < b) := (compare(a, b) == -1)", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(LT(a, b) == (CMP(a, b) == -1));
}

test("(5) (a > b) := (compare(a, b) == 1)", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(GT(a, b) == (CMP(a, b) == 1));
}

test("(6) (a == b) := (compare(a, b) == 0)", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(EQ(a, b) == (CMP(a, b) == 0));
}

test("(7) (min(a, b) == if a <= b then a else b) := true)",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(EQ(MIN(a, b), (LE(a, b) ? a : b)));
}

test("(8) (max(a, b) == if a >= b then a else b) := true)",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(EQ(MAX(a, b), (GE(a, b) ? a : b)));
}
