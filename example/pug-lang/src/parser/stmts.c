/* -*- coding: utf-8-unix -*- */

#include "parser/expr.h"

// PARSER(Expr) stmts(void);
parsec(stmts, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) semi = lexme(char1(';'));
  DO() {
    SCAN(stmt(), lhs);
    SCAN(optional(semi), m);
    if (m.none) {
      RETURN(lhs);
    }
    SCAN(optional(stmts()), rhs);
    if (rhs.none) {
      RETURN(lhs);
    }
    RETURN(E.seq(lhs, rhs.value));
  }
}

PARSER(Expr) stmt(void) {
  return choice(decl(), expr());
}
