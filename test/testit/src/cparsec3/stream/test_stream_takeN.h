/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/parsec/base/parsec.h>

// #define _DISABLE_TESTIT_OPERATORS
#define TestSuite Stream(S)
#include "testit.h"

#define stream trait(Stream(S))
#define Toks Maybe(Tuple(Tokens(S), S))

static inline bool toks_eq(Toks a, Toks b) {
  if (a.none) {
    return b.none;
  }
  if (b.none) {
    return a.none;
  }
  return eq(a.value.e1, b.value.e1) && eq(a.value.e2, b.value.e2);
}

test("if (n <= 0) := true, then "
     "takeN(n, input) returns a 0-length chunk and `input`.",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  int n = x->n;
  S input = x->input;
  if (n <= 0) {
    Toks expect = x->expect;
    Toks actual = stream.takeN(n, input);
    c_assert(!expect.none);
    c_assert(!actual.none);
    c_assert(toks_eq(expect, actual));
  }
}

test("if (n > 0) && null(input) := true, then "
     "takeN(n, input) returns nothing. (i.e. end of input)",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  int n = x->n;
  S input = x->input;
  if (n > 0 && stream.null(input)) {
    Toks actual = stream.takeN(n, input);
    c_assert(actual.none);
  }
}

test("if (n > 0) && !null(input) := true, then "
     "takeN(n, input) returns a chunk whose length is n at most "
     "and the rest of `input`.",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  int n = x->n;
  S input = x->input;
  if (n > 0 && !stream.null(input)) {
    Toks expect = x->expect;
    Toks actual = stream.takeN(n, input);
    c_assert(!expect.none);
    c_assert(!actual.none);
    c_assert(toks_eq(expect, actual));
  }
}
