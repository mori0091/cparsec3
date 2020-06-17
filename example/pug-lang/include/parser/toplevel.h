/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "blank.h"
#include "expr.h"

C_API_BEGIN

// program    = stmts eof

PARSER(Expr) program(void);

C_API_END
