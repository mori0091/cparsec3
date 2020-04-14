/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "expr.h"

C_API_BEGIN

// program    = stmt-list eof
// stmt-list  = stmt {stmt}
// stmt       = let | stmt1
// stmt1      = block | empty_stmt | expr_stmt
// let        = "let" variable "=" equality
// block      = "{" stmt-list "}"
// empty_stmt = ";"
// expr_stmt  = expr ";"

PARSER(Expr) program(void);

PARSER(Expr) stmt_list(void);
PARSER(Expr) stmt(void);
PARSER(Expr) stmt1(void);
PARSER(Expr) let(void);
PARSER(Expr) block(void);
PARSER(Expr) empty_stmt(void);
PARSER(Expr) expr_stmt(void);

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
  ArrayT(Expr) A = trait(Array(Expr));
  DO() {
    SCAN(some(stmt()), a);
    Expr* p = A.end(a) - 1;
    Expr q = *p;
    while (p != A.begin(a)) {
      q = E.seq(*--p, q);
    }
    RETURN(q);
  }
}

PARSER(Expr) stmt(void) {
  return choice(let(), stmt1());
}

PARSER(Expr) stmt1(void) {
  return choice(block(), empty_stmt(), expr_stmt());
}

// PARSER(Expr) let(void);
parsec(let, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) op = lexme(char1('='));
  PARSER(char) semi = lexme(char1(';'));
  DO() {
    SCAN(lexme(keyword("let")));
    SCAN(variable(), lhs);
    SCAN(op);
    SCAN(equality(), rhs);
    SCAN(semi);
    RETURN(E.let(lhs, rhs));
  }
}

// PARSER(Expr) empty_stmt(void);
parsec(empty_stmt, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) semi = lexme(char1(';'));
  DO() {
    SCAN(semi);
    RETURN(E.unit());
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

// PARSER(Expr) expr_stmt(void);
parsec(expr_stmt, Expr) {
  PARSER(char) semi = lexme(char1(';'));
  DO() {
    SCAN(expr(), x);
    SCAN(semi);
    RETURN(x);
  }
}

#endif
