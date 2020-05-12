/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Subst.h"
#include "Type.h"
#include "TypeVarProc.h"

typedef struct TypeScheme {
  int numTGen; ///< number of `TGen` in the `type`.
  Type type;   ///< type prototype.
} TypeScheme;

trait_List(TypeScheme);
trait_TypeVarProc(TypeScheme);
trait_TypeVarProc(List(TypeScheme));
