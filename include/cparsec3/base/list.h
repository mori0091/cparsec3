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
  typedef_List(T);                                                       \
  typedef struct {                                                       \
    List(T) (*cons)(T x, List(T) xs);                                    \
    void (*free)(List(T) xs);                                            \
    List(T) (*drop)(size_t n, List(T) xs);                               \
    T (*head)(List(T) xs);                                               \
    List(T) (*tail)(List(T) xs);                                         \
    bool (*null)(List(T) xs);                                            \
  } ListT(T);                                                            \
  ListT(T) Trait(List(T));                                               \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_List(T)                                                     \
  C_API_BEGIN                                                            \
  trait_Mem(stList(T));                                                  \
  impl_Mem(stList(T));                                                   \
  static List(T) FUNC_NAME(cons, List(T))(T x, List(T) xs) {             \
    List(T) ys = trait(Mem(stList(T))).create(1);                        \
    ys->head = x;                                                        \
    ys->tail = xs;                                                       \
    return ys;                                                           \
  }                                                                      \
  static void FUNC_NAME(free, List(T))(List(T) xs) {                     \
    while (xs) {                                                         \
      List(T) ys = xs;                                                   \
      xs = xs->tail;                                                     \
      *ys = (stList(T)){0};                                              \
      trait(Mem(stList(T))).free(ys);                                    \
    }                                                                    \
  }                                                                      \
  static List(T) FUNC_NAME(drop, List(T))(size_t n, List(T) xs) {        \
    while (xs && n) {                                                    \
      List(T) ys = xs;                                                   \
      xs = xs->tail;                                                     \
      *ys = (stList(T)){0};                                              \
      trait(Mem(stList(T))).free(ys);                                    \
      n--;                                                               \
    }                                                                    \
    return xs;                                                           \
  }                                                                      \
  static T FUNC_NAME(head, List(T))(List(T) xs) {                        \
    return xs->head;                                                     \
  }                                                                      \
  static List(T) FUNC_NAME(tail, List(T))(List(T) xs) {                  \
    return xs->tail;                                                     \
  }                                                                      \
  static bool FUNC_NAME(null, List(T))(List(T) xs) {                     \
    return !xs;                                                          \
  }                                                                      \
  ListT(T) Trait(List(T)) {                                              \
    return (ListT(T)){                                                   \
        .cons = FUNC_NAME(cons, List(T)),                                \
        .free = FUNC_NAME(free, List(T)),                                \
        .drop = FUNC_NAME(drop, List(T)),                                \
        .head = FUNC_NAME(head, List(T)),                                \
        .tail = FUNC_NAME(tail, List(T)),                                \
        .null = FUNC_NAME(null, List(T)),                                \
    };                                                                   \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS
