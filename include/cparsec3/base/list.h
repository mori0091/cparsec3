/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#include "eq.h"
#include "ord.h"

#include "mem.h"

#include "itr.h"
#include "slice.h"

#define List(T) TYPE_NAME(List, T)
#define stList(T) TYPE_NAME(stList, T)
#define ListT(T) TYPE_NAME(ListT, T)

// -----------------------------------------------------------------------
#define trait_List(T)                                                    \
  C_API_BEGIN                                                            \
  /* ---- List(T) */                                                     \
  typedef struct stList(T) * List(T);                                    \
  typedef struct stList(T) stList(T);                                    \
  struct stList(T) {                                                     \
    struct stList(T) * tail;                                             \
    T head;                                                              \
  };                                                                     \
  /* ---- trait List(T) */                                               \
  typedef struct ListT(T) ListT(T);                                      \
  struct ListT(T) {                                                      \
    List(T) empty;                                                       \
    bool (*null)(List(T) xs);                                            \
    size_t (*length)(List(T) xs);                                        \
    void (*reverse)(List(T) * pxs);                                      \
    void (*free)(List(T) xs);                                            \
    List(T) (*cons)(T x, List(T) xs);                                    \
    List(T) (*drop)(size_t n, List(T) xs);                               \
    T (*head)(List(T) xs);                                               \
    List(T) (*tail)(List(T) xs);                                         \
    List(T) (*from_array)(size_t n, T * a);                              \
  };                                                                     \
  ListT(T) Trait(List(T));                                               \
  /* ---- instance Eq(List(T)) */                                        \
  trait_Eq(List(T));                                                     \
  /* ---- instance Ord(List(T)) */                                       \
  trait_Ord(List(T));                                                    \
  /* ---- instance Itr(List(T)) */                                       \
  typedef T Item(List(T));                                               \
  typedef struct Itr(List(T)) Itr(List(T));                              \
  struct Itr(List(T)) {                                                  \
    List(T) xs;                                                          \
  };                                                                     \
  trait_Itr(List(T));                                                    \
  /* ---- instance Slice(List(T)) */                                     \
  trait_Slice(List(T));                                                  \
  /* ---- */                                                             \
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
  static void FUNC_NAME(reverse, List(T))(List(T) * pxs) {               \
    assert(pxs);                                                         \
    if (!*pxs) {                                                         \
      return;                                                            \
    }                                                                    \
    List(T) xs = *pxs;                                                   \
    List(T) ys = xs->tail;                                               \
    xs->tail = NULL;                                                     \
    while (ys) {                                                         \
      List(T) zs = ys->tail;                                             \
      ys->tail = xs;                                                     \
      xs = ys;                                                           \
      ys = zs;                                                           \
    }                                                                    \
    *pxs = xs;                                                           \
  }                                                                      \
  static void FUNC_NAME(free, List(T))(List(T) xs) {                     \
    while (xs) {                                                         \
      List(T) ys = xs;                                                   \
      xs = xs->tail;                                                     \
      *ys = (stList(T)){0};                                              \
      trait(Mem(stList(T))).free(ys);                                    \
    }                                                                    \
  }                                                                      \
  static List(T) FUNC_NAME(cons, List(T))(T x, List(T) xs) {             \
    List(T) ys = trait(Mem(stList(T))).create(1);                        \
    ys->head = x;                                                        \
    ys->tail = xs;                                                       \
    return ys;                                                           \
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
  static List(T) FUNC_NAME(from_array, List(T))(size_t n, T * a) {       \
    List(T) xs = NULL;                                                   \
    while (n) {                                                          \
      xs = FUNC_NAME(cons, List(T))(a[--n], xs);                         \
    }                                                                    \
    return xs;                                                           \
  }                                                                      \
  ListT(T) Trait(List(T)) {                                              \
    return (ListT(T)){                                                   \
        .empty = NULL,                                                   \
        .null = FUNC_NAME(null, List(T)),                                \
        .length = FUNC_NAME(length, List(T)),                            \
        .reverse = FUNC_NAME(reverse, List(T)),                          \
        .free = FUNC_NAME(free, List(T)),                                \
        .cons = FUNC_NAME(cons, List(T)),                                \
        .drop = FUNC_NAME(drop, List(T)),                                \
        .head = FUNC_NAME(head, List(T)),                                \
        .tail = FUNC_NAME(tail, List(T)),                                \
        .from_array = FUNC_NAME(from_array, List(T)),                    \
    };                                                                   \
  }                                                                      \
  /* ---- instance Eq(List(T)) */                                        \
  static bool FUNC_NAME(eq, Eq(List(T)))(List(T) a, List(T) b) {         \
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
  instance_Eq(List(T), FUNC_NAME(eq, Eq(List(T))));                      \
  /* ---- instance Ord(List(T)) */                                       \
  static int FUNC_NAME(cmp, Ord(List(T)))(List(T) a, List(T) b) {        \
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
  instance_Ord(List(T), FUNC_NAME(cmp, Ord(List(T))));                   \
  /* ---- instance Itr(List(T))*/                                        \
  static Itr(List(T)) FUNC_NAME(itr, Itr(List(T)))(List(T) xs) {         \
    return (Itr(List(T))){.xs = xs};                                     \
  }                                                                      \
  static T* FUNC_NAME(ptr, Itr(List(T)))(Itr(List(T)) it) {              \
    return (it.xs ? &(it.xs->head) : 0);                                 \
  }                                                                      \
  static Itr(List(T)) FUNC_NAME(next, Itr(List(T)))(Itr(List(T)) it) {   \
    assert(it.xs);                                                       \
    it.xs = it.xs->tail;                                                 \
    return it;                                                           \
  }                                                                      \
  instance_Itr(List(T), FUNC_NAME(itr, Itr(List(T))),                    \
               FUNC_NAME(ptr, Itr(List(T))),                             \
               FUNC_NAME(next, Itr(List(T))));                           \
  /* ---- instance Slice(List(T)) */                                     \
  instance_Slice(List(T));                                               \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
