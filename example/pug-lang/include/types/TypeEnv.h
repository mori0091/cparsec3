/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#include "Expr.h"
#include "Type.h"

typedef struct TypeEnv {
  /** apply type environment (i.e. context) */
  Type (*judge)(Context ctx, Expr e);
} TypeEnv;

TypeEnv Trait(TypeEnv);
