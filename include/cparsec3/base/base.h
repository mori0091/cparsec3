/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "none.h"
#include "typeset.h"

#include "eq.h"
#include "ord.h"
#include "show.h"

#include "mem.h"

#include "iterable.h"
#include "itr.h"

#include "array.h"
#include "list.h"
#include "maybe.h"

#include "result.h"
#include "tuple.h"

#include "function.h"
#include "funcptr.h"

FOREACH(trait_Eq, TYPESET(ALL));
FOREACH(trait_Ord, TYPESET(ALL));
FOREACH(trait_Show, TYPESET(ALL));

FOREACH(trait_Mem, TYPESET(ALL));
FOREACH(trait_Array, TYPESET(ALL));
FOREACH(trait_List, TYPESET(ALL));
FOREACH(trait_Maybe, TYPESET(ALL));

FOREACH(trait_Eq, APPLY(Array, TYPESET(ALL)));
FOREACH(trait_Eq, APPLY(List, TYPESET(ALL)));
FOREACH(trait_Eq, APPLY(Maybe, TYPESET(ALL)));

FOREACH(trait_Ord, APPLY(Array, TYPESET(ALL)));
FOREACH(trait_Ord, APPLY(List, TYPESET(ALL)));
FOREACH(trait_Ord, APPLY(Maybe, TYPESET(ALL)));

FOREACH(trait_Show, APPLY(Array, TYPESET(ALL)));
FOREACH(trait_Show, APPLY(List, TYPESET(ALL)));
FOREACH(trait_Show, APPLY(Maybe, TYPESET(ALL)));

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

FOREACH(trait_Eq, APPLY(Array, TYPESET_CONTAINER));
FOREACH(trait_Eq, APPLY(List, TYPESET_CONTAINER));
FOREACH(trait_Eq, APPLY(Maybe, TYPESET_CONTAINER));

FOREACH(trait_Ord, APPLY(Array, TYPESET_CONTAINER));
FOREACH(trait_Ord, APPLY(List, TYPESET_CONTAINER));
FOREACH(trait_Ord, APPLY(Maybe, TYPESET_CONTAINER));

FOREACH(trait_Show, APPLY(Array, TYPESET_CONTAINER));
FOREACH(trait_Show, APPLY(List, TYPESET_CONTAINER));
FOREACH(trait_Show, APPLY(Maybe, TYPESET_CONTAINER));

#endif
