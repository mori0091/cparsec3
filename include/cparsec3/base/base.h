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

#include "funcptr.h"
#include "function.h"

// components
#define TYPESET_0 TYPESET(ALL)

FOREACH(trait_Eq, TYPESET_0);
FOREACH(trait_Ord, TYPESET_0);
FOREACH(trait_Show, TYPESET_0);

// containers (e.g. Array(T), List(T), etc.)
#define TYPESET_1 TYPESET_ARRAY_1, TYPESET_LIST_1, TYPESET_MAYBE_1
#define TYPESET_ARRAY_1 APPLY(Array, TYPESET_0)
#define TYPESET_LIST_1 APPLY(List, TYPESET_0)
#define TYPESET_MAYBE_1 APPLY(Maybe, TYPESET_0)

FOREACH(trait_Mem, TYPESET_0);
FOREACH(trait_Array, TYPESET_0);
FOREACH(trait_List, TYPESET_0);
FOREACH(trait_Maybe, TYPESET_0);

FOREACH(trait_Eq, TYPESET_1);
FOREACH(trait_Ord, TYPESET_1);
FOREACH(trait_Show, TYPESET_1);

// nested containers
#define TYPESET_2 APPLY(Maybe, TYPESET_ARRAY_1)

FOREACH(trait_Maybe, TYPESET_ARRAY_1);

FOREACH(trait_Eq, TYPESET_2);
FOREACH(trait_Ord, TYPESET_2);
FOREACH(trait_Show, TYPESET_2);

// #define TYPESET_2 TYPESET_ARRAY_2, TYPESET_LIST_2, TYPESET_MAYBE_2
// #define TYPESET_ARRAY_2 APPLY(Array, TYPESET_1)
// #define TYPESET_LIST_2 APPLY(List, TYPESET_1)
// #define TYPESET_MAYBE_2 APPLY(Maybe, TYPESET_1)

// FOREACH(trait_Mem, TYPESET_1);
// FOREACH(trait_Array, TYPESET_1);
// FOREACH(trait_List, TYPESET_1);
// FOREACH(trait_Maybe, TYPESET_1);

// FOREACH(trait_Eq, TYPESET_2);
// FOREACH(trait_Ord, TYPESET_2);
// FOREACH(trait_Show, TYPESET_2);
