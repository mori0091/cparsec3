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

#define TYPESET_0 TYPESET(ALL)
#define TYPESET_1                                                        \
  APPLY(Array, TYPESET_0), APPLY(List, TYPESET_0), APPLY(Maybe, TYPESET_0)

FOREACH(trait_Mem, TYPESET_0);
FOREACH(trait_Array, TYPESET_0);
FOREACH(trait_List, TYPESET_0);
FOREACH(trait_Maybe, TYPESET_0);

FOREACH(trait_Mem, TYPESET_1);
FOREACH(trait_Array, TYPESET_1);
FOREACH(trait_List, TYPESET_1);
FOREACH(trait_Maybe, TYPESET_1);
