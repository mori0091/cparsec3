/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "none.h"
#include "typeset.h"

#include "eq.h"
#include "ord.h"

#include "mem.h"

#include "itr.h"

#include "array.h"
#include "list.h"
#include "maybe.h"

#include "result.h"
#include "tuple.h"

#include "data.h"

FOREACH(trait_Mem, TYPESET(ALL));
FOREACH(trait_Array, TYPESET(ALL));
FOREACH(trait_List, TYPESET(ALL));
FOREACH(trait_Maybe, TYPESET(ALL));

#if !defined(CPARSEC_ENABLE_NESTED_CONTAINER)
#define TYPESET_COMPONENT TYPESET(ALL)
#else
#define TYPESET_COMPONENT TYPESET(ALL), TYPESET_CONTAINER
#define TYPESET_CONTAINER                                                \
  APPLY(Array, TYPESET(ALL)), APPLY(List, TYPESET(ALL)),                 \
      APPLY(Maybe, TYPESET(ALL))

FOREACH(trait_Mem, TYPESET_CONTAINER);
FOREACH(trait_Array, TYPESET_CONTAINER);
FOREACH(trait_List, TYPESET_CONTAINER);
FOREACH(trait_Maybe, TYPESET_CONTAINER);
#endif
