/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#include "types/Expr.h"

typedef_Result(Expr, String);

Result(Expr, String) eval(Expr e);
