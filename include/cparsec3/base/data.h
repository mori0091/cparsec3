/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "none.h"
#include "typeset.h"

#include "array.h"
#include "list.h"
#include "mem.h"

#define Data(...) TYPE_NAME(Data, __VA_ARGS__)
// -----------------------------------------------------------------------
#define trait_Data(T)                                                    \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  trait_Mem(T);                                                         \
  trait_Array(T);                                                        \
  trait_List(T);                                                         \
  /* ---- */                                                             \
  typedef struct {                                                       \
    MemT(T) Mem;                                                         \
    ArrayT(T) Array;                                                     \
    ListT(T) List;                                                       \
  } Data(T);                                                             \
  /* ---- */                                                             \
  Data(T) Trait(Data(T));                                                \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Data(T)                                                     \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  impl_Mem(T);                                                           \
  impl_Array(T);                                                         \
  impl_List(T);                                                          \
  /* ---- */                                                             \
  Data(T) Trait(Data(T)) {                                               \
    return (Data(T)){.Mem = trait(Mem(T)),                               \
                     .Array = trait(Array(T)),                           \
                     .List = trait(List(T))};                            \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
