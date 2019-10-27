/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/core/core.h>

// #define _DISABLE_TESTIT_OPERATORS
#define TestSuite Stream(S)
#include "testit.h"

#define stream cparsec_module(Stream(S))
#define Toks Maybe(Tuple(Tokens(S), S))

static inline bool toks_eq(Toks a, Toks b) {
  if (a.none) {
    return b.none;
  }
  if (b.none) {
    return a.none;
  }
  tie((Tokens(S) a_chunk, S a_rest), a.value);
  tie((Tokens(S) b_chunk, S b_rest), b.value);
  return eq(a_chunk.length, b_chunk.length) &&
         !strncmp(a_chunk.value, b_chunk.value, a_chunk.length) &&
         eq(a_rest, b_rest);
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

test("if (n > 0) && empty(input) := true, then "
     "takeN(n, input) returns nothing. (i.e. end of input)",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  int n = x->n;
  S input = x->input;
  if (n > 0 && stream.empty(input)) {
    Toks actual = stream.takeN(n, input);
    c_assert(actual.none);
  }
}

test("if (n > 0) && !empty(input) := true, then "
     "takeN(n, input) returns a chunk whose length is n at most "
     "and the rest of `input`.",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  int n = x->n;
  S input = x->input;
  if (n > 0 && !stream.empty(input)) {
    Toks expect = x->expect;
    Toks actual = stream.takeN(n, input);
    c_assert(!expect.none);
    c_assert(!actual.none);
    c_assert(toks_eq(expect, actual));
  }
}
