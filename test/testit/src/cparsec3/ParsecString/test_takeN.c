/* -*- coding: utf-8-unix -*- */
#define TestSuite ParsecString
#include "testit.h"

#include <cparsec3/ParsecString.h>

#define S cparsec_module(Stream(String))

test("takeN (n < 0) from an empty stream") {
  Maybe(Tupple(Tokens(String), String)) r;
  {
    String input = "";
    {
      r = S.takeN(-1, input);
      c_assert(!r.none);
      c_assert(eq(r.value.first.length, 0));
      c_assert(eq(r.value.first.value, ""));
      c_assert(eq(r.value.second, ""));
    }
  }
}

test("takeN (n = 0) from an empty stream") {
  Maybe(Tupple(Tokens(String), String)) r;
  {
    String input = "";
    {
      r = S.takeN(0, input);
      c_assert(!r.none);
      c_assert(eq(r.value.first.length, 0));
      c_assert(eq(r.value.first.value, ""));
      c_assert(eq(r.value.second, ""));
    }
  }
}

test("takeN (n > 0) from an empty stream") {
  Maybe(Tupple(Tokens(String), String)) r;
  {
    String input = "";
    {
      r = S.takeN(1, input);
      c_assert(r.none);
    }
  }
}

test("takeN (n < 0) from non empty stream") {
  Maybe(Tupple(Tokens(String), String)) r;
  {
    String input = "abc";
    {
      r = S.takeN(-1, input);
      c_assert(!r.none);
      c_assert(eq(r.value.first.length, 0));
      c_assert(eq(r.value.first.value, ""));
      c_assert(eq(r.value.second, "abc"));
    }
  }
}

test("takeN (n = 0) from non empty stream") {
  Maybe(Tupple(Tokens(String), String)) r;
  {
    String input = "abc";
    {
      r = S.takeN(0, input);
      c_assert(!r.none);
      c_assert(eq(r.value.first.length, 0));
      c_assert(eq(r.value.first.value, ""));
      c_assert(eq(r.value.second, "abc"));
    }
  }
}

test("takeN (n > 0) from non empty stream") {
  Maybe(Tupple(Tokens(String), String)) r;
  {
    String input = "abc";
    {
      r = S.takeN(1, input);
      c_assert(!r.none);
      c_assert(eq(r.value.first.length, 1));
      c_assert(!strncmp(r.value.first.value, "a", 1));
      c_assert(eq(r.value.second, "bc"));
    }
    {
      r = S.takeN(2, input);
      c_assert(!r.none);
      c_assert(eq(r.value.first.length, 2));
      c_assert(!strncmp(r.value.first.value, "ab", 2));
      c_assert(eq(r.value.second, "c"));
    }
    {
      r = S.takeN(3, input);
      c_assert(!r.none);
      c_assert(eq(r.value.first.length, 3));
      c_assert(!strncmp(r.value.first.value, "abc", 3));
      c_assert(eq(r.value.second, ""));
    }
    {
      r = S.takeN(4, input);
      c_assert(!r.none);
      c_assert(eq(r.value.first.length, 3));
      c_assert(!strncmp(r.value.first.value, "abc", 3));
      c_assert(eq(r.value.second, ""));
    }
  }
}
