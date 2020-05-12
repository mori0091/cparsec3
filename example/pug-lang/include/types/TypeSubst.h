/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#include "Type.h"

/** creates fresh/unique type variable */
Tyvar freshTyvar(void);

// -----------------------------------------------------------------------
// type substitution ; a map from `type variable` to `type`

typedef struct TypeSubstEntry {
  Tyvar tvar;
  Type type;
} TypeSubstEntry;

trait_List(TypeSubstEntry);

typedef List(TypeSubstEntry) TypeSubst;

// -----------------------------------------------------------------------
// ---- trait TypeSubst

typedef struct TypeSubstT {
  /** empty type-substitution */
  TypeSubst empty;
  /** creates a type-substitution consists of one entry. */
  TypeSubst (*create)(Tyvar tvar, Type type);
  /** Composites two type-substitutions. */
  TypeSubst (*composite)(TypeSubst s1, TypeSubst s2);
} TypeSubstT;

TypeSubstT Trait(TypeSubst);
