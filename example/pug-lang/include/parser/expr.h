/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "lexme.h"
#include "optional.h"

#include "number.h"

C_API_BEGIN

// expr     = assign
// assign   = equality {"=" equality}
// equality = ordered {("==" | "!=") ordered}
// ordered  = addsub {("<=" | "<" | ">" | ">=") addsub}
// addsub   = muldiv {("+" | "-") muldiv}
// muldiv   = unary {("*" | "/") unary}
// unary    = [("+" | "-" | "!")] primary
// primary  = paren | number | variable
// paren    = "(" expr ")"
// variable = identifier
//
// identifier  = identStart{identLetter}
// identStart  = "_" | letter
// identLetter = "_" | alphaNum

PARSER(Expr) expr(void);
PARSER(Expr) assign(void);
PARSER(Expr) equality(void);
PARSER(Expr) ordered(void);
PARSER(Expr) addsub(void);
PARSER(Expr) muldiv(void);
PARSER(Expr) unary(void);
PARSER(Expr) primary(void);
PARSER(Expr) paren(void);

PARSER(Expr) variable(void);
PARSER(String) identifier(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

PARSER(Expr) expr(void) {
  return assign();
}

// PARSER(Expr) assign(void);
parsec(assign, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = equality();
  PARSER(char) op = lexme(char1('='));
  DO() {
    SCAN(p, lhs);
    if (lhs->type != VAR) {
      RETURN(lhs);
    }
    SCAN(optional(op), m);
    if (m.none) {
      RETURN(lhs);
    }
    SCAN(p, rhs);
    RETURN(E.assign(lhs, rhs));
  }
}

// PARSER(Expr) equality(void);
parsec(equality, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = ordered();
  PARSER(String) op = lexme(either(string1("=="), string1("!=")));
  DO() {
    SCAN(p, lhs);
    SCAN(optional(op), m);
    if (m.none) {
      RETURN(lhs);
    }
    SCAN(p, rhs);
    lhs = (g_eq("==", m.value) ? E.eq : E.neq)(lhs, rhs);
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
    SCAN(optional(op1), lg);
    if (lg.none) {
      RETURN(lhs);
    }
    SCAN(optional(op2), e);
    SCAN(space());
    SCAN(p, rhs);
    if (e.none) {
      lhs = ((lg.value == '<') ? E.lt : E.gt)(lhs, rhs);
    } else {
      lhs = ((lg.value == '<') ? E.le : E.ge)(lhs, rhs);
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
    SCAN(choice(paren(), number(), variable()), x);
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

// PARSER(Expr) variable(void);
parsec(variable, Expr) {
  DO() {
    SCAN(lexme(identifier()), x);
    RETURN(trait(Expr).var((Var){x}));
  }
}

// PARSER(String) identifier0(void);
parsec(identifier0, String) {
  PARSER(char) identStart = either(char1('_'), letter());
  PARSER(char) identLetter = either(char1('_'), alphaNum());

  DO() {
    SCAN(identStart, x);
    SCAN(many(identLetter), xs);
    SCAN(space());
    size_t len = 1 + xs.length;
    char* cs = mem_malloc(len + 1);
    cs[0] = x;
    memmove(cs + 1, xs.data, xs.length);
    cs[len] = 0;
    g_free(xs);
    RETURN(cs);
  }
}

PARSER(String) identifier(void) {
  return label("identifier", identifier0());
}

#endif

C_API_END
