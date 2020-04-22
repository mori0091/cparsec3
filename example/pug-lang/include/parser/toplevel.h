/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "blank.h"
#include "expr.h"

C_API_BEGIN

// program    = stmts eof

PARSER(Expr) program(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

// PARSER(Expr) program(void);
parsec(program, Expr) {
  DO() {
    SCAN(blank());
    SCAN(stmts(), x);
    SCAN(eof());
    RETURN(x);
  }
}

#endif

C_API_END
