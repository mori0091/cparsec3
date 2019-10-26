/* -*- coding: utf-8-unix -*- */
#define _DISABLE_TESTIT_OPERATORS
#define TestSuite Eq_uint64_t
#include "testit.h"

#include <cparsec3/base/base.h>

#define EQ cparsec_module(Eq(uint64_t)).eq
#define NEQ cparsec_module(Eq(uint64_t)).neq

test("eq((uint64_t)1, (uint64_t)1)") {
  c_assert(EQ(1, 1));
}

test("eq((uint64_t)1, (uint64_t)0)", .should_fail = true) {
  c_assert(EQ(1, 0));
}

test("neq((uint64_t)1, (uint64_t)1)", .should_fail = true) {
  c_assert(NEQ(1, 1));
}

test("neq((uint64_t)1, (uint64_t)0)") {
  c_assert(NEQ(1, 0));
}
