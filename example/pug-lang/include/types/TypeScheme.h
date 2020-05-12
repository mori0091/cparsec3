/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Subst.h"
#include "Type.h"
#include "Types.h"

typedef struct TypeScheme {
  int numTGen; ///< number of `TGen` in the `type`.
  Type type;   ///< type prototype.
} TypeScheme;

trait_List(TypeScheme);
trait_Types(TypeScheme);
trait_Types(List(TypeScheme));
