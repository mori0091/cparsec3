/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "expr.h"

C_API_BEGIN

// program    = stmt-list eof
// stmt-list  = stmt {stmt}
// stmt       = defvar | stmt1
// stmt1      = block | empty_stmt | expr_stmt
// defvar     = "var" variable "=" equality
// block      = "{" stmt-list "}"
// empty_stmt = ";"
// expr_stmt  = expr ";"

PARSER(Expr) program(void);

PARSER(Expr) stmt_list(void);
PARSER(Expr) stmt(void);
PARSER(Expr) stmt1(void);
PARSER(Expr) defvar(void);
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
  return choice(defvar(), stmt1());
}

PARSER(Expr) stmt1(void) {
  return choice(block(), empty_stmt(), expr_stmt());
}

parsec(keyword0, String, String) {
  PARSER(char) identLetter = either(char1('_'), alphaNum());
  DO() WITH(s) {
    SCAN(string1(s));
    SCAN(optional(identLetter), m);
    if (!m.none) {
      FAIL("?");
    }
    RETURN(s);
  }
}

PARSER(String) keyword(String s) {
  return tryp(keyword0(s));
}

// PARSER(Expr) defvar(void);
parsec(defvar, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) op = lexme(char1('='));
  PARSER(char) semi = lexme(char1(';'));
  DO() {
    SCAN(lexme(keyword("let")));
    SCAN(variable(), lhs);
    SCAN(op);
    SCAN(equality(), rhs);
    SCAN(semi);
    RETURN(E.defvar(lhs, rhs));
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
