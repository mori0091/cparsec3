/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base_generics.h>

#include "Type.h"

#include "Subst.h"
#include "Types.h"

#include "Scheme.h"

#include "Assump.h"
#include "Unify.h"

#define TRAIT_TYPES(T) trait(Types(T))

#define GENERIC_TYPES(t)                                                 \
  GENERIC(t, IDENTITY, TRAIT_TYPES, Type, Scheme, Assump, List(Type),    \
          List(Scheme), List(Assump))

// Subst
#define t_empty_subst() trait(Subst).empty
#define t_create_subst(tvar, type) trait(Subst).create(tvar, type)
#define t_composite_subst(s1, s2) trait(Subst).composite(s1, s2)

// Types
#define t_apply_subst(s, t) GENERIC_TYPES(t).subst(s, t)
#define t_extract_tvars(t) GENERIC_TYPES(t).tvarsOf(t)
#define t_union_tvars(tvars1, tvars2) unionTyvars(tvars1, tvars2)

// Scheme / Assump
#define t_gen(as, type) trait(Assumption).scheme(as, type)
#define t_find(var, as) trait(Assumption).lookup(var, as)
#define t_add(var, scheme, as) trait(Assumption).add(var, scheme, as)

// Unify
#define t_unifier(type1, type2) trait(Unify).unifier(type1, type2)
#define t_tbind(tvar, type) trait(Unify).tbind(tvar, type)
