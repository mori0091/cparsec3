/* -*- coding: utf-8-unix -*- */
#define _DISABLE_TESTIT_OPERATORS
#define TestSuite Eq_String
#include "testit.h"

#include <cparsec3/base/base.h>

#define EQ cparsec_module(Eq(String)).eq
#define NEQ cparsec_module(Eq(String)).neq

test("eq(\"abc\", \"abc\")") {
  c_assert(EQ("abc", "abc"));
}

test("eq(\"abc\", \"abcd\")", .should_fail = true) {
  c_assert(EQ("abc", "abcd"));
}

test("eq(\"abcd\", \"abc\")", .should_fail = true) {
  c_assert(EQ("abc", "abcd"));
}

test("eq(\"abc\", \"ABC\")", .should_fail = true) {
  c_assert(EQ("abc", "ABC"));
}

test("neq(\"abc\", \"abc\")", .should_fail = true) {
  c_assert(NEQ("abc", "abc"));
}

test("neq(\"abc\", \"abcd\")") {
  c_assert(NEQ("abc", "abcd"));
}

test("neq(\"abcd\", \"abc\")") {
  c_assert(NEQ("abc", "abcd"));
}

test("neq(\"abc\", \"ABC\")") {
  c_assert(NEQ("abc", "ABC"));
}
