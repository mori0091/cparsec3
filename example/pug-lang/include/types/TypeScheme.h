/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Type.h"
#include "TypeSubst.h"
#include "TypeVarProc.h"

typedef struct TypeScheme {
  int numAny;                   ///< number of `TANY` in the `type`.
  Type type;                    ///< type prototype.
} TypeScheme;

trait_List(TypeScheme);
trait_TypeVarProc(TypeScheme);
trait_TypeVarProc(List(TypeScheme));