/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#include "Pat.h"

typedef struct Expr* Expr;

typedef struct Alt {
  List(Pat) ps;
  Expr e;
} Alt;

decl_user_type(Alt);

trait_List(Alt);
decl_user_type(List(Alt));
