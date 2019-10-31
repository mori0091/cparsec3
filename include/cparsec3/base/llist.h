/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "mem.h"

#define LList(T) TYPE_NAME(LList, T)
#define stLList(T) TYPE_NAME(stLList, T)
// -----------------------------------------------------------------------
#define typedef_LList(T)                                                 \
  C_API_BEGIN                                                            \
  typedef struct LList(T) {                                              \
    struct LList(T) * tail;                                              \
    T head;                                                              \
  }                                                                      \
  stLList(T), *LList(T);                                                 \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define trait_DataLList(T)                                               \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    LList(T) (*cons)(T, LList(T));                                       \
    void (*drop)(LList(T));                                              \
  } Data(LList(T));                                                      \
  Data(LList(T)) Trait(Data(LList(T)));                                  \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_DataLList(T)                                                \
  C_API_BEGIN                                                            \
  static LList(T) FUNC_NAME(cons, LList(T))(T x, LList(T) xs) {          \
    LList(T) ys = trait(Mem(stLList(T))).create(1);                      \
    ys->head = x;                                                        \
    ys->tail = xs;                                                       \
    return ys;                                                           \
  }                                                                      \
  static void FUNC_NAME(drop, LList(T))(LList(T) xs) {                   \
    while (xs) {                                                         \
      LList(T) ys = xs;                                                  \
      xs = xs->tail;                                                     \
      /* trait(Mem(T)).free(ys->head); */                                \
      *ys = (stLList(T)){0};                                             \
      trait(Mem(stLList(T))).free(ys);                                   \
    }                                                                    \
  }                                                                      \
  Data(LList(T)) Trait(Data(LList(T))) {                                 \
    return (Data(LList(T))){.cons = FUNC_NAME(cons, LList(T)),           \
                            .drop = FUNC_NAME(drop, LList(T))};          \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS
