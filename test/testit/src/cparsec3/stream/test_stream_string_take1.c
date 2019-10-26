/* -*- coding: utf-8-unix -*- */
#include <cparsec3/stream/stream_string.h>

#define s String

#define TestSuite Stream(s)
#include "testit.h"

#define S cparsec_module(Stream(s))
#define Tok Maybe(Tupple(Token(s), s))

test("if !empty(input) := true, then "
     "take1(input) returns the 1st token and the rest of 'input'.") {
  s input;
  Tok r;
  {
    input = "abc";
    r = S.take1(input);
    c_assert(!r.none);
    c_assert(eq(r.value.first, 'a'));
    c_assert(eq(r.value.second, "bc"));
  }
  {
    input = r.value.second;
    r = S.take1(input);
    c_assert(!r.none);
    c_assert(eq(r.value.first, 'b'));
    c_assert(eq(r.value.second, "c"));
  }
  {
    input = r.value.second;
    r = S.take1(input);
    c_assert(!r.none);
    c_assert(eq(r.value.first, 'c'));
    c_assert(eq(r.value.second, ""));
  }
}

test("if empty(input) := true, then "
     "take1(input) returns nothing.") {
  s input;
  Tok r;
  {
    input = "";
    r = S.take1(input);
    c_assert(r.none);
  }
}
