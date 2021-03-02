/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base_generics.h>

#define _DISABLE_TESTIT_OPERATORS
#define TestSuite CAT(Ord(Array(char)), 2)
#include "testit.h"

test("(4.1) (a < b) := (compare(a, b) == -1) where b is clone of a") {
  Array(char) a = g_array(char, 'a', 'b', 'c');
  Array(char) b = g_array(char, 'a', 'b', 'c');
  c_assert(g_lt(a, b) == (g_cmp(a, b) == -1));
  c_assert(!g_lt(a, b));
  c_assert(g_cmp(a, b) == 0);
}

test("(5.1) (a > b) := (compare(a, b) == 1) where b is clone of a") {
  Array(char) a = g_array(char, 'a', 'b', 'c');
  Array(char) b = g_array(char, 'a', 'b', 'c');
  c_assert(g_gt(a, b) == (g_cmp(a, b) == 1));
  c_assert(!g_gt(a, b));
  c_assert(g_cmp(a, b) == 0);
}

test("(6.1) (a == b) := (compare(a, b) == 0) where b is clone of a") {
  Array(char) a = g_array(char, 'a', 'b', 'c');
  Array(char) b = g_array(char, 'a', 'b', 'c');
  c_assert(g_eq(a, b) == (g_cmp(a, b) == 0));
  c_assert(g_eq(a, b));
  c_assert(g_cmp(a, b) == 0);
}
