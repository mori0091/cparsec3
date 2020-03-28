/* -*- coding: utf-8-unix -*- */

#define _DISABLE_TESTIT_OPERATORS
#define TestSuite ParsecRepeat
#include "testit.h"

#include "myparsec.h"

#define S CPARSEC_STREAM_TYPE

test("[parser] count(-1, digit()), \"\" -> empty ok") {
  ParseResult(S, Array(char)) r = runParser(count(-1, digit()), "");
  c_assert(r.success);
  c_assert(g_eq(r.ok, trait(Array(char)).empty));
  c_assert(g_eq(r.state, ""));
}

test("[parser] count(0, digit()), \"\" -> empty ok") {
  ParseResult(S, Array(char)) r = runParser(count(0, digit()), "");
  c_assert(r.success);
  c_assert(g_eq(r.ok, trait(Array(char)).empty));
  c_assert(g_eq(r.state, ""));
}

test("[parser] count(3, digit()), \"\" -> empty error") {
  ParseResult(S, Array(char)) r = runParser(count(3, digit()), "");
  c_assert(!r.success);
  c_assert(g_eq(r.state, ""));
}

test("[parser] count(3, digit()), \"123abc\" -> consumed ok") {
  ParseResult(S, Array(char)) r = runParser(count(3, digit()), "123abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(char, '1', '2', '3')));
  c_assert(g_eq(r.state, "abc"));
}

test("[parser] count(2, digit()), \"123abc\" -> consumed ok") {
  ParseResult(S, Array(char)) r = runParser(count(2, digit()), "123abc");
  c_assert(r.success);
  c_assert(g_eq(r.ok, g_array(char, '1', '2')));
  c_assert(g_eq(r.state, "3abc"));
}

test("[parser] count(4, digit()), \"123abc\" -> consumed error") {
  ParseResult(S, Array(char)) r = runParser(count(4, digit()), "123abc");
  c_assert(!r.success);
  c_assert(g_eq(r.state, "abc"));
}
