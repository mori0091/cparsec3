/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#include "Type.h"

/** creates fresh/unique type variable */
Tyvar freshTyvar(void);

// -----------------------------------------------------------------------
// type substitution ; a map from `type variable` to `type`

typedef struct SubstEntry {
  Tyvar tvar;
  Type type;
} SubstEntry;

trait_List(SubstEntry);

typedef List(SubstEntry) Subst;

// -----------------------------------------------------------------------
// ---- trait Subst

typedef struct SubstT {
  /** empty type-substitution */
  Subst empty;
  /** creates a type-substitution consists of one entry. */
  Subst (*create)(Tyvar tvar, Type type);
  /** Composites two type-substitutions. */
  Subst (*composite)(Subst s1, Subst s2);
} SubstT;

SubstT Trait(Subst);
