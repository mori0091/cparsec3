/* -*- coding: utf-8-unix -*- */

#define _DISABLE_TESTIT_OPERATORS
#define TestSuite ParsecRepeat
#include "testit.h"

#include "myparsec.h"

#define S CPARSEC_STREAM_TYPE

test("[parser] many(digit()), \"\" -> empty ok") {
  ParseResult(S, Array(char)) r = runParser(many(digit()), "");
  c_assert(r.success);
  c_assert(g_eq(r.ok, trait(Array(char)).empty));
  c_assert(g_eq(r.state, ""));
}

test("[parser] many(digit()), \"abc\" -> empty ok") {
  ParseResult(S, Array(char)) r = runParser(many(digit()), "abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, trait(Array(char)).empty));
  c_assert(g_eq(r.state, "abc"));
}

test("[parser] many(digit()), \"123abc\" -> consumed ok") {
  ParseResult(S, Array(char)) r = runParser(many(digit()), "123abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(char, '1', '2', '3')));
  c_assert(g_eq(r.state, "abc"));
}

test("[parser] many(string1(\"123\")), \"123abc\" -> consumed ok") {
  ParseResult(S, Array(String)) r =
      runParser(many(string1("123")), "123abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(String, "123")));
  c_assert(g_eq(r.state, "abc"));
}

test("[parser] many(string1(\"ab\")), \"ababc\" -> consumed ok") {
  ParseResult(S, Array(String)) r =
      runParser(many(string1("ab")), "ababc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(String, "ab", "ab")));
  c_assert(g_eq(r.state, "c"));
}

test("[parser] many(string1(\"ab\")), \"abaBc\" -> consumed ok") {
  ParseResult(S, Array(String)) r =
      runParser(many(string1("ab")), "abaBc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(String, "ab")));
  c_assert(g_eq(r.state, "aBc"));
}

test("[parser] many(string1(\"\")), \"abc\" -> empty error") {
  ParseResult(S, Array(String)) r = runParser(many(string1("")), "abc");
  c_assert(!r.success);
  c_assert(g_eq(r.state, "abc"));
}

fn(abcFn, UnParserArgs(S, String)) {
  DO() {
    SCAN(char1('a'));
    SCAN(char1('b'));
    SCAN(char1('c'));
    RETURN("abc");
  }
}

static Parsec(S, String) abc(void) {
  return (Parsec(S, String)){abcFn()};
}

test("[parser] many(abc()), \"abc\" -> consumed ok") {
  ParseResult(S, Array(String)) r = runParser(many(abc()), "abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(String, "abc")));
  c_assert(g_eq(r.state, ""));
}

test("[parser] many(abc()), \"abcaBc\" -> consumed error") {
  ParseResult(S, Array(String)) r = runParser(many(abc()), "abcaBc");
  c_assert(!r.success);
  c_assert(g_eq(r.state, "Bc"));
}
