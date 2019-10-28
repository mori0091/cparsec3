/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "module.h"
#include "typeset.h"

#define Eq(T) TYPE_NAME(Eq, T)
#define declare_Eq(T)                                                    \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    bool (*eq)(T, T);                                                    \
    bool (*neq)(T, T);                                                   \
  } Eq(T);                                                               \
  Eq(T) CPARSEC_MODULE(Eq(T))(void);                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

FOREACH(declare_Eq, TYPESET(ALL));
