/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/core/core.h>

// #define _DISABLE_TESTIT_OPERATORS
#define TestSuite Stream(S)
#include "testit.h"

#define stream cparsec_module(Stream(S))
#define Tok Maybe(Tuple(Token(S), S))

test("if !empty(input) := true, then "
     "take1(input) returns the 1st token and the rest of 'input'.",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  S input = x->input;
  if (!stream.empty(input)) {
    Tok expect = x->expect;
    Tok actual = stream.take1(input);
    c_assert(!expect.none);
    c_assert(!actual.none);
    c_assert(eq(expect.value.first, actual.value.first));
    c_assert(eq(expect.value.second, actual.value.second));
  }
}

test("if empty(input) := true, then "
     "take1(input) returns nothing.",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  S input = x->input;
  if (stream.empty(input)) {
    Tok expect = x->expect;
    Tok actual = stream.take1(input);
    c_assert(expect.none);
    c_assert(actual.none);
  }
}
