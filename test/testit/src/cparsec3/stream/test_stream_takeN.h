/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/core/core.h>

// #define _DISABLE_TESTIT_OPERATORS
#define TestSuite Stream(S)
#include "testit.h"

#define stream cparsec_module(Stream(S))
#define Toks Maybe(Tupple(Tokens(S), S))

test("if (n <= 0) := true, then "
     "takeN(n, input) returns a 0-length chunk and `input`.",
     .generator = T_GENERATOR) {
  struct data* x = testit_current_test_data();
  int n = x->n;
  S input = x->input;
  if (n <= 0) {
    Toks actual = stream.takeN(n, input);
    c_assert(!actual.none);
    c_assert(eq(actual.value.first.length, 0));
    c_assert(!strncmp("", actual.value.first.value, 0));
    c_assert(eq(input, actual.value.second));
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
    c_assert(!actual.none);
    c_assert(eq(expect.value.first.length, actual.value.first.length));
    c_assert(!strncmp(expect.value.first.value, actual.value.first.value,
                      expect.value.first.length));
    c_assert(eq(expect.value.second, actual.value.second));
  }
}
