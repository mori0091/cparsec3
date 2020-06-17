/* -*- coding: utf-8-unix -*- */

#include "parser/toplevel.h"

// PARSER(Expr) program(void);
parsec(program, Expr) {
  DO() {
    SCAN(blank());
    SCAN(stmts(), x);
    SCAN(eof());
    RETURN(x);
  }
}
