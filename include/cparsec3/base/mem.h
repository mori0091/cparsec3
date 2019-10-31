/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#define Mem(T) TYPE_NAME(Mem, T)
// -----------------------------------------------------------------------
#define trait_Mem(T)                                                     \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    T* (*create)(size_t);                                                \
    T* (*recreate)(T*, size_t);                                          \
    void (*free)(T*);                                                    \
  } Mem(T);                                                              \
  Mem(T) Trait(Mem(T));                                                  \
  C_API_END                                                              \
  END_OF_STATEMENTS

FOREACH(trait_Mem, TYPESET(ALL));
