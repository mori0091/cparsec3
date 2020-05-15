/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Type.h"
#include "Types.h"

typedef struct Pred Pred;
struct Pred {
  String ident;
  Type type;
};

Pred IsIn(String ident, Type type);

trait_Eq(Pred);

trait_List(Pred);
trait_Eq(List(Pred));

trait_Types(Pred);
trait_Types(List(Pred));
