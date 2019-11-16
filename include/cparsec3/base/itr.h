/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#define Item(C) TYPE_NAME(Item, C)
#define Itr(C) TYPE_NAME(Itr, C)
#define ItrT(C) TYPE_NAME(ItrT, C)
// -----------------------------------------------------------------------
#define trait_Itr(C)                                                     \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    Itr(C) (*itr)(C c);                                                  \
    bool (*null)(Itr(C) it);                                             \
    Itr(C) (*next)(Itr(C) it);                                           \
    Item(C) (*get)(Itr(C) it);                                           \
    void (*set)(Item(C) x, Itr(C) it);                                   \
  } ItrT(C);                                                             \
  ItrT(C) Trait(Itr(C));                                                 \
  C_API_END                                                              \
  END_OF_STATEMENTS
