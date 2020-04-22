/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

C_API_BEGIN

PARSER(Expr) number(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

#include <limits.h>

// PARSER(Expr) number0(void);
parsec(number0, Expr) {
  DO() {
    SCAN(some(digit()), xs);
    ArrayT(char) A = trait(Array(char));
    int64_t y = 0;
    for (char* p = A.begin(xs); p != A.end(xs); p++) {
      int64_t x = *p - '0';
      if (y > INT64_MAX / 10 ||
          (y == INT64_MAX / 10 && x > INT64_MAX % 10)) {
        FAIL("too large number");
      }
      y = 10 * y + x;
    }
    RETURN(trait(Expr).num((Num){y}));
  }
}

PARSER(Expr) number(void) {
  return label("a number [0..2^63-1]", tryp(number0()));
}

#endif

C_API_END
