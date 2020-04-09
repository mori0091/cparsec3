/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "lexme.h"
#include "optional.h"

#include "number.h"

C_API_BEGIN

// expr     = equality
// equality = ordered {("==" | "!=") ordered}
// ordered  = addsub {("<=" | "<" | ">" | ">=") addsub}
// addsub   = muldiv {("+" | "-") muldiv}
// muldiv   = unary {("*" | "/") unary}
// unary    = [("+" | "-" | "!")] primary
// primary  = paren | number
// paren    = "(" expr ")"

PARSER(Expr) expr(void);
PARSER(Expr) equality(void);
PARSER(Expr) ordered(void);
PARSER(Expr) addsub(void);
PARSER(Expr) muldiv(void);
PARSER(Expr) unary(void);
PARSER(Expr) primary(void);
PARSER(Expr) paren(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

PARSER(Expr) expr(void) {
  return equality();
}

// PARSER(Expr) equality(void);
parsec(equality, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = ordered();
  PARSER(String) op = lexme(either(string1("=="), string1("!=")));
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op), m);
      if (m.none) {
        break;
      }
      SCAN(p, rhs);
      lhs = (g_eq("==", m.value) ? E.eq : E.neq)(lhs, rhs);
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) ordered(void);
parsec(ordered, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = addsub();
  PARSER(char) op1 = either(char1('<'), char1('>'));
  PARSER(char) op2 = char1('=');
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op1), lg);
      if (lg.none) {
        break;
      }
      SCAN(optional(op2), e);
      SCAN(space());
      SCAN(p, rhs);
      lhs = ((e.none) ? ((lg.value == '<') ? E.lt : E.gt)
                      : ((lg.value == '<') ? E.le : E.ge))(lhs, rhs);
    }
    RETURN(lhs);
  }
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
      lhs = ((m.value == '+') ? E.add : E.sub)(lhs, rhs);
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) muldiv(void);
parsec(muldiv, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = unary();
  PARSER(char) op = lexme(choice(char1('*'), char1('/'), char1('%')));
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op), m);
      if (m.none) {
        break;
      }
      SCAN(p, rhs);
      switch (m.value) {
      case '*':
        lhs = E.mul(lhs, rhs);
        break;
      case '/':
        lhs = E.div(lhs, rhs);
        break;
      case '%':
        lhs = E.mod(lhs, rhs);
        break;
      default:
        FAIL("Unexpected behaviour");
        break;
      }
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
    if (m.none) {
      RETURN(rhs);
    }
    switch (m.value) {
    case '+':
      RETURN(rhs);
    case '-':
      RETURN(E.neg(rhs));
    case '!':
      RETURN(E.not(rhs));
    default:
      FAIL("Unexpected behaviour");
    }
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
