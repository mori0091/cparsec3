/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "lexme.h"
#include "optional.h"

#include "number.h"

C_API_BEGIN

PARSER(Expr) expr(void);
PARSER(Expr) addsub(void);
PARSER(Expr) muldiv(void);
PARSER(Expr) unary(void);
PARSER(Expr) primary(void);
PARSER(Expr) paren(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

PARSER(Expr) expr(void) {
  return addsub();
}

// PARSER(Expr) addsub(void);
parsec(addsub, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = muldiv();
  PARSER(char) op = lexme(either(char1('+'), char1('-')));
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op), m);
      if (m.none) {
        break;
      }
      SCAN(p, rhs);
      lhs = (m.value == '+') ? E.add(lhs, rhs) : E.sub(lhs, rhs);
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) muldiv(void);
parsec(muldiv, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = unary();
  PARSER(char) op = lexme(either(char1('*'), char1('/')));
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op), m);
      if (m.none) {
        break;
      }
      SCAN(p, rhs);
      lhs = (m.value == '*') ? E.mul(lhs, rhs) : E.div(lhs, rhs);
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) unary(void);
parsec(unary, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = primary();
  PARSER(char) op = lexme(choice(char1('+'), char1('-'), char1('!')));
  DO() {
    SCAN(optional(op), m);
    SCAN(p, rhs);
    if (m.none || m.value == '+') {
      RETURN(rhs);
    }
    if (m.value == '-') {
      RETURN(E.neg(rhs));
    }
    if (m.value == '!') {
      RETURN(E.not(rhs));
    }
    assert(0);
  }
}

// PARSER(Expr) primary(void);
parsec(primary, Expr) {
  DO() {
    SCAN(either(paren(), number()), x);
    SCAN(space());
    RETURN(x);
  }
}

// PARSER(Expr) paren(void);
parsec(paren, Expr) {
  DO() {
    SCAN(lexme(char1('(')));
    SCAN(expr(), x);
    SCAN(lexme(char1(')')));
    RETURN(x);
  }
}

#endif

C_API_END
