/* -*- coding: utf-8-unix -*- */
#define _DISABLE_TESTIT_OPERATORS
#define TestSuite Eq_char
#include "testit.h"

#include <cparsec3/base/base.h>

#define EQ cparsec_module(Eq(char)).eq
#define NEQ cparsec_module(Eq(char)).neq

test("eq((char)'a', (char)'a')") {
  c_assert(EQ('a', 'a'));
}

test("eq((char)'a', (char)'b')", .should_fail = true) {
  c_assert(EQ('a', 'b'));
}

test("neq((char)'a', (char)'a')", .should_fail = true) {
  c_assert(NEQ('a', 'a'));
}

test("neq((char)'a', (char)'b')") {
  c_assert(NEQ('a', 'b'));
}
