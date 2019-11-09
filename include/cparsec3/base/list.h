/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "mem.h"

#define List(T) TYPE_NAME(List, T)
#define stList(T) TYPE_NAME(stList, T)
#define ListT(T) TYPE_NAME(ListT, T)
// -----------------------------------------------------------------------
#define typedef_List(T)                                                  \
  C_API_BEGIN                                                            \
  typedef struct List(T) {                                               \
    struct List(T) * tail;                                               \
    T head;                                                              \
  }                                                                      \
  stList(T), *List(T);                                                   \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define trait_List(T)                                                    \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    List(T) (*cons)(T, List(T));                                         \
    void (*drop)(List(T));                                               \
  } ListT(T);                                                            \
  ListT(T) Trait(List(T));                                               \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_List(T)                                                     \
  C_API_BEGIN                                                            \
  static List(T) FUNC_NAME(cons, List(T))(T x, List(T) xs) {             \
    List(T) ys = trait(Mem(stList(T))).create(1);                        \
    ys->head = x;                                                        \
    ys->tail = xs;                                                       \
    return ys;                                                           \
  }                                                                      \
  static void FUNC_NAME(drop, List(T))(List(T) xs) {                     \
    while (xs) {                                                         \
      List(T) ys = xs;                                                   \
      xs = xs->tail;                                                     \
      /* trait(Mem(T)).free(ys->head); */                                \
      *ys = (stList(T)){0};                                              \
      trait(Mem(stList(T))).free(ys);                                    \
    }                                                                    \
  }                                                                      \
  Data(List(T)) Trait(Data(List(T))) {                                   \
    return (Data(List(T))){.cons = FUNC_NAME(cons, List(T)),             \
                           .drop = FUNC_NAME(drop, List(T))};            \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS
