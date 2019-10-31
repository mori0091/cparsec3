/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "typeset.h"

#define Maybe(T) TYPE_NAME(Maybe, T)
#define typedef_Maybe(T)                                                 \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    bool none;                                                           \
    T value;                                                             \
  } Maybe(T);                                                            \
  C_API_END                                                              \
  END_OF_STATEMENTS

FOREACH(typedef_Maybe, TYPESET(ALL));

// Maybe(T) is an instance of type-class Eq
#include "eq.h"
FOREACH(trait_Eq, APPLY(Maybe, TYPESET(ALL)));

// Maybe(T) is an instance of type-class Ord
#include "ord.h"
FOREACH(trait_Ord, APPLY(Maybe, TYPESET(ALL)));
