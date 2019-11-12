/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#include "eq.h"
#include "ord.h"

#include "mem.h"

#define List(T) TYPE_NAME(List, T)
#define stList(T) TYPE_NAME(stList, T)
#define ListT(T) TYPE_NAME(ListT, T)
// -----------------------------------------------------------------------
#define typedef_List(T)                                                  \
  C_API_BEGIN                                                            \
  typedef struct stList(T) {                                             \
    struct stList(T) * tail;                                             \
    T head;                                                              \
  }                                                                      \
  stList(T), *List(T);                                                   \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define trait_List(T)                                                    \
  C_API_BEGIN                                                            \
  typedef_List(T);                                                       \
  trait_Eq(List(T));                                                     \
  trait_Ord(List(T));                                                    \
  typedef struct {                                                       \
    List(T) empty;                                                       \
    bool (*null)(List(T) xs);                                            \
    size_t (*length)(List(T) xs);                                        \
    List(T) (*cons)(T x, List(T) xs);                                    \
    List(T) (*from_array)(size_t n, T* a);                               \
    void (*free)(List(T) xs);                                            \
    List(T) (*drop)(size_t n, List(T) xs);                               \
    T (*head)(List(T) xs);                                               \
    List(T) (*tail)(List(T) xs);                                         \
  } ListT(T);                                                            \
  ListT(T) Trait(List(T));                                               \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_List(T)                                                     \
  C_API_BEGIN                                                            \
  /* ---- trait List(T) */                                               \
  trait_Mem(stList(T));                                                  \
  impl_Mem(stList(T));                                                   \
  static bool FUNC_NAME(null, List(T))(List(T) xs) {                     \
    return !xs;                                                          \
  }                                                                      \
  static size_t FUNC_NAME(length, List(T))(List(T) xs) {                 \
    size_t len = 0;                                                      \
    for (; xs; xs = xs->tail) {                                          \
      len++;                                                             \
    }                                                                    \
    return len;                                                          \
  }                                                                      \
  static List(T) FUNC_NAME(cons, List(T))(T x, List(T) xs) {             \
    List(T) ys = trait(Mem(stList(T))).create(1);                        \
    ys->head = x;                                                        \
    ys->tail = xs;                                                       \
    return ys;                                                           \
  }                                                                      \
  static List(T) FUNC_NAME(from_array, List(T))(size_t n, T * a) {       \
    List(T) xs = NULL;                                                   \
    while (n) {                                                          \
      xs = FUNC_NAME(cons, List(T))(a[--n], xs);                         \
    }                                                                    \
    return xs;                                                           \
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
  ListT(T) Trait(List(T)) {                                              \
    return (ListT(T)){                                                   \
        .empty = NULL,                                                   \
        .null = FUNC_NAME(null, List(T)),                                \
        .length = FUNC_NAME(length, List(T)),                            \
        .cons = FUNC_NAME(cons, List(T)),                                \
        .from_array = FUNC_NAME(from_array, List(T)),                    \
        .free = FUNC_NAME(free, List(T)),                                \
        .drop = FUNC_NAME(drop, List(T)),                                \
        .head = FUNC_NAME(head, List(T)),                                \
        .tail = FUNC_NAME(tail, List(T)),                                \
    };                                                                   \
  }                                                                      \
  /* ---- instance Eq(List(T)) */                                        \
  static bool FUNC_NAME(eq, List(T))(List(T) a, List(T) b) {             \
    for (;;) {                                                           \
      if (a == b) {                                                      \
        return true;                                                     \
      }                                                                  \
      if (!a || !b) {                                                    \
        return false;                                                    \
      }                                                                  \
      if (trait(Eq(T)).neq(a->head, b->head)) {                          \
        return false;                                                    \
      }                                                                  \
      a = a->tail;                                                       \
      b = b->tail;                                                       \
    }                                                                    \
  }                                                                      \
  instance_Eq(List(T), FUNC_NAME(eq, List(T)));                          \
  /* ---- instance Ord(List(T)) */                                       \
  static int FUNC_NAME(cmp, List(T))(List(T) a, List(T) b) {             \
    for (;;) {                                                           \
      if (a == b) {                                                      \
        return 0;                                                        \
      }                                                                  \
      if (!a) {                                                          \
        return -1;                                                       \
      }                                                                  \
      if (!b) {                                                          \
        return 1;                                                        \
      }                                                                  \
      int o = trait(Ord(T)).cmp(a->head, b->head);                       \
      if (o) {                                                           \
        return o;                                                        \
      }                                                                  \
      a = a->tail;                                                       \
      b = b->tail;                                                       \
    }                                                                    \
  }                                                                      \
  instance_Ord(List(T), FUNC_NAME(cmp, List(T)));                        \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
// FOREACH(trait_List, TYPESET(ALL));
