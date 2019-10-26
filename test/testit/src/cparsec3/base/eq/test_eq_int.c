/* -*- coding: utf-8-unix -*- */
#define _DISABLE_TESTIT_OPERATORS
#define TestSuite Eq_int
#include "testit.h"

#include <cparsec3/base/base.h>

#define EQ cparsec_module(Eq(int)).eq
#define NEQ cparsec_module(Eq(int)).neq

test("eq((int)1, (int)1)") {
  c_assert(EQ(1, 1));
}

test("eq((int)1, (int)0)", .should_fail = true) {
  c_assert(EQ(1, 0));
}

test("neq((int)1, (int)1)", .should_fail = true) {
  c_assert(NEQ(1, 1));
}

test("neq((int)1, (int)0)") {
  c_assert(NEQ(1, 0));
}
