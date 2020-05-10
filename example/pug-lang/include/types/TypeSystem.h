/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base_generics.h>

#include "Type.h"
#include "TypeEnv.h"

#include "TypeSubst.h"
#include "TypeVarProc.h"

#include "TypeScheme.h"

#include "TypeInfer.h"

#include "TypeAssumption.h"
#include "TypeUnify.h"

#define TRAIT_TYPEVARPROC(T) trait(TypeVarProc(T))

#define GENERIC_TYPEVARPROC(t)                                           \
  GENERIC(t, IDENTITY, TRAIT_TYPEVARPROC, Type, TypeScheme,              \
          TypeAssumption, List(Type), List(TypeScheme),                  \
          List(TypeAssumption))

#define TRAIT_TIRUNNER(T) trait(TIRunner(T))

#define GENERIC_TIRUNNER(ti)                                             \
  GENERIC(ti, TypeInfer, TRAIT_TIRUNNER, TYPE_INFER_RETURN_TYPES())

// TypeSubst
#define t_empty_subst() trait(TypeSubst).empty
#define t_create_subst(tvar, type) trait(TypeSubst).create(tvar, type)
#define t_composite_subst(s1, s2) trait(TypeSubst).composite(s1, s2)

// TypeVarProc
#define t_apply_subst(s, t) GENERIC_TYPEVARPROC(t).subst(s, t)
#define t_extract_tvars(t) GENERIC_TYPEVARPROC(t).tvarsOf(t)
#define t_union_tvars(tvars1, tvars2) unionTVars(tvars1, tvars2)

// TypeInfer / TIRunner (type inference monad)
#define runTypeInferP(ti, st) GENERIC_TIRUNNER(ti).pRunTypeInferP(ti, st)
#define runTypeInfer(ti) GENERIC_TIRUNNER(ti).pRunTypeInfer(ti)

// TypeScheme / TypeAssumption
#define t_gen(as, type) trait(Assumption).scheme(as, type)
#define t_find(var, as) trait(Assumption).lookup(var, as)
#define t_add(var, scheme, as) trait(Assumption).add(var, scheme, as)

// TypeUnify
#define t_unifier(type1, type2) trait(TypeUnify).unifier(type1, type2)
#define t_tbind(tvar, type) trait(TypeUnify).tbind(tvar, type)
