/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/core/core.h>

// #define _DISABLE_TESTIT_OPERATORS
#define TestSuite Stream(S)
#include "testit.h"

#define stream trait(Stream(S))
#define Tok Maybe(Tuple(Token(S), S))

static inline bool tok_eq(Tok a, Tok b) {
  if (a.none) {
    return b.none;
  }
  if (b.none) {
    return a.none;
  }
  tie((Token(S) a_token, S a_rest), a.value);
  tie((Token(S) b_token, S b_rest), b.value);
  return eq(a_token, b_token) && eq(a_rest, b_rest);
}

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
    c_assert(tok_eq(expect, actual));
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
