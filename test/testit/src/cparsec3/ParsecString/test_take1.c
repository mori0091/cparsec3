/* -*- coding: utf-8-unix -*- */
#define TestSuite ParsecString
#include "testit.h"

#include <cparsec3/ParsecString.h>

#define S cparsec_module(Stream(String))

test("take1") {
  Maybe(Tupple(Token(String), String)) r;
  String input;
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
  {
    input = r.value.second;
    r = S.take1(input);
    c_assert(r.none);
  }
}
