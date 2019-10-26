/* -*- coding: utf-8-unix -*- */
#define _DISABLE_TESTIT_OPERATORS
#define TestSuite Eq_int8_t
#include "testit.h"

#include <cparsec3/base/base.h>

#define EQ cparsec_module(Eq(int8_t)).eq
#define NEQ cparsec_module(Eq(int8_t)).neq

test("eq((int8_t)1, (int8_t)1)") {
  c_assert(EQ(1, 1));
}

test("eq((int8_t)1, (int8_t)0)", .should_fail = true) {
  c_assert(EQ(1, 0));
}

test("neq((int8_t)1, (int8_t)1)", .should_fail = true) {
  c_assert(NEQ(1, 1));
}

test("neq((int8_t)1, (int8_t)0)") {
  c_assert(NEQ(1, 0));
}
