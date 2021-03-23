/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#include "types/Type.h"
#include "types/Expr.h"

typedef_Result(Type, String);

Result(Type, String) typeOf(Expr e);
