/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "expr.h"

C_API_BEGIN

PARSER(Expr) stmt(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

// PARSER(Expr) stmt(void);
parsec(stmt, Expr) {
  DO() {
    SCAN(expr(), x);
    SCAN(eof());
    RETURN(x);
  }
}

#endif
