/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#define _DISABLE_TESTIT_OPERATORS
#define TestSuite Eq(T)
#include "testit.h"

#define E trait(Eq(T))
#define O trait(Ord(T))

#define EQ E.eq   /* == E.equal */
#define NEQ E.neq /* == E.notEqual */

#define NA true

test("Reflexivity: (a == a) := true", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  c_assert(EQ(a, a));
}

test("Symmetry: (a == b) := (b == a)", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(EQ(a, b) == EQ(b, a));
}

test("Transitivity: if (a == b && b == c) := true, then (a == c) := true",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  T c = x->c;
  c_assert((EQ(a, b) && EQ(b, c)) ? EQ(a, c) : NA);
}

test("Substitutivity: if (a == b) := true and f is a function whose "
     "return type is an instance of Eq, then (f(a) == f(b)) := true",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  T (*f)(T) = x->f;
  c_assert(EQ(a, b) ? EQ(f(a), f(b)) : NA);
}

test("Negation: (a != b) := !(a == b)", .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  T a = x->a;
  T b = x->b;
  c_assert(NEQ(a, b) == !EQ(a, b));
}
