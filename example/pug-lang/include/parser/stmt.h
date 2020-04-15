/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "expr.h"

C_API_BEGIN

// program    = stmt-list eof
// stmt-list  = stmt {";" [stmt]}
// stmt       = let | block | expr
// let        = "let" variable "=" expr0 ";"
// block      = "{" stmt-list "}"

PARSER(Expr) program(void);

PARSER(Expr) stmt_list(void);
PARSER(Expr) stmt(void);
PARSER(Expr) let(void);
PARSER(Expr) block(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

// PARSER(Expr) program(void);
parsec(program, Expr) {
  DO() {
    SCAN(stmt_list(), x);
    SCAN(eof());
    RETURN(x);
  }
}

// PARSER(Expr) stmt_list(void);
parsec(stmt_list, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) semi = lexme(char1(';'));
  DO() {
    SCAN(stmt(), lhs);
    SCAN(optional(semi), m);
    if (m.none) {
      RETURN(lhs);
    }
    SCAN(optional(stmt_list()), rhs);
    if (rhs.none) {
      RETURN(lhs);
    }
    RETURN(E.seq(lhs, rhs.value));
  }
}

PARSER(Expr) stmt(void) {
  return choice(let(), block(), expr());
}

// PARSER(Expr) let(void);
parsec(let, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) op = lexme(char1('='));
  DO() {
    SCAN(lexme(keyword("let")));
    SCAN(variable(), lhs);
    SCAN(op);
    SCAN(expr0(), rhs);
    RETURN(E.let(lhs, rhs));
  }
}

// PARSER(Expr) block(void);
parsec(block, Expr) {
  PARSER(char) open_brace = lexme(char1('{'));
  PARSER(char) close_brace = lexme(char1('}'));
  DO() {
    SCAN(open_brace);
    SCAN(stmt_list(), x);
    SCAN(close_brace);
    RETURN(x);
  }
}

#endif
