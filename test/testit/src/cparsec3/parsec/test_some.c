/* -*- coding: utf-8-unix -*- */

#define _DISABLE_TESTIT_OPERATORS
#define TestSuite ParsecRepeat
#include "testit.h"

#include "myparsec.h"

#define S CPARSEC_STREAM_TYPE

test("[parser] some(digit()), \"\" -> empty error") {
  ParseResult(S, Array(char)) r = runParser(some(digit()), "");
  c_assert(!r.success);
  c_assert(g_eq(r.state, ""));
}

test("[parser] some(digit()), \"abc\" -> empty error") {
  ParseResult(S, Array(char)) r = runParser(some(digit()), "abc");
  c_assert(!r.success);
  c_assert(g_eq(r.state, "abc"));
}

test("[parser] some(digit()), \"123abc\" -> consumed ok") {
  ParseResult(S, Array(char)) r = runParser(some(digit()), "123abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(char, '1', '2', '3')));
  c_assert(g_eq(r.state, "abc"));
}

test("[parser] some(string1(\"123\")), \"123abc\" -> consumed ok") {
  ParseResult(S, Array(String)) r =
      runParser(some(string1("123")), "123abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(String, "123")));
  c_assert(g_eq(r.state, "abc"));
}

test("[parser] some(string1(\"ab\")), \"ababc\" -> consumed ok") {
  ParseResult(S, Array(String)) r =
      runParser(some(string1("ab")), "ababc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(String, "ab", "ab")));
  c_assert(g_eq(r.state, "c"));
}

test("[parser] some(string1(\"ab\")), \"abaBc\" -> consumed ok") {
  ParseResult(S, Array(String)) r =
      runParser(some(string1("ab")), "abaBc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(String, "ab")));
  c_assert(g_eq(r.state, "aBc"));
}

test("[parser] some(string1(\"\")), \"abc\" -> empty error") {
  ParseResult(S, Array(String)) r = runParser(some(string1("")), "abc");
  c_assert(!r.success);
  c_assert(g_eq(r.state, "abc"));
}

test("[parser] some(abc()), \"abc\" -> consumed ok") {
  ParseResult(S, Array(String)) r = runParser(some(abc()), "abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(String, "abc")));
  c_assert(g_eq(r.state, ""));
}

test("[parser] some(abc()), \"abcaBc\" -> consumed error") {
  ParseResult(S, Array(String)) r = runParser(some(abc()), "abcaBc");
  c_assert(!r.success);
  c_assert(g_eq(r.state, "Bc"));
}
