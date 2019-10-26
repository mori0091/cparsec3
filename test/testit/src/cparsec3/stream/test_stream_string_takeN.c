/* -*- coding: utf-8-unix -*- */
#include <cparsec3/stream/stream_string.h>

#define s String

#define TestSuite Stream(s)
#include "testit.h"

#define S cparsec_module(Stream(s))
#define Toks Maybe(Tupple(Tokens(s), s))

test("if (n < 0) && empty(input) := true, then "
     "takeN(n, input) returns a 0-length chunk and `input`.") {
  s input = "";
  {
    Toks r = S.takeN(-1, input);
    c_assert(!r.none);
    c_assert(eq(r.value.first.length, 0));
    c_assert(eq(r.value.first.value, ""));
    c_assert(eq(r.value.second, ""));
  }
}

test("if (n == 0) && empty(input) := true, then "
     "takeN(n, input) returns a 0-length chunk and `input`.") {
  s input = "";
  {
    Toks r = S.takeN(0, input);
    c_assert(!r.none);
    c_assert(eq(r.value.first.length, 0));
    c_assert(eq(r.value.first.value, ""));
    c_assert(eq(r.value.second, ""));
  }
}

test("if (n > 0) && empty(input) := true, then "
     "takeN(n, input) returns nothing. (i.e. end of input)") {
  s input = "";
  {
    Toks r = S.takeN(1, input);
    c_assert(r.none);
  }
}

test("if (n < 0) && !empty(input) := true, then "
     "takeN(n, input) returns a 0-length chunk and `input`.") {
  s input = "abc";
  {
    Toks r = S.takeN(-1, input);
    c_assert(!r.none);
    c_assert(eq(r.value.first.length, 0));
    c_assert(eq(r.value.first.value, ""));
    c_assert(eq(r.value.second, "abc"));
  }
}

test("if (n == 0) && !empty(input) := true, then "
     "takeN(n, input) returns a 0-length chunk and `input`.") {
  s input = "abc";
  {
    Toks r = S.takeN(0, input);
    c_assert(!r.none);
    c_assert(eq(r.value.first.length, 0));
    c_assert(eq(r.value.first.value, ""));
    c_assert(eq(r.value.second, "abc"));
  }
}

test("if (n > 0) && !empty(input) := true, then "
     "takeN(n, input) returns a chunk whose length is n at most "
     "and the rest of `input`.") {
  s input = "abc";
  {
    Toks r = S.takeN(1, input);
    c_assert(!r.none);
    c_assert(eq(r.value.first.length, 1));
    c_assert(!strncmp(r.value.first.value, "a", 1));
    c_assert(eq(r.value.second, "bc"));
  }
  {
    Toks r = S.takeN(2, input);
    c_assert(!r.none);
    c_assert(eq(r.value.first.length, 2));
    c_assert(!strncmp(r.value.first.value, "ab", 2));
    c_assert(eq(r.value.second, "c"));
  }
  {
    Toks r = S.takeN(3, input);
    c_assert(!r.none);
    c_assert(eq(r.value.first.length, 3));
    c_assert(!strncmp(r.value.first.value, "abc", 3));
    c_assert(eq(r.value.second, ""));
  }
  {
    Toks r = S.takeN(4, input);
    c_assert(!r.none);
    c_assert(eq(r.value.first.length, 3));
    c_assert(!strncmp(r.value.first.value, "abc", 3));
    c_assert(eq(r.value.second, ""));
  }
}
