/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "eq.h"
#include "ord.h"

#include "mem.h"

#include "iterable.h"
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
    void (*free)(List(T) * pxs);                                         \
    List(T) (*cons)(T x, List(T) xs);                                    \
    List(T) (*drop)(size_t n, List(T) xs);                               \
    T (*head)(List(T) xs);                                               \
    List(T) (*tail)(List(T) xs);                                         \
    List(T) (*from_array)(size_t n, T * a);                              \
  };                                                                     \
  ListT(T) Trait(List(T));                                               \
  /* ---- instance Itr(List(T)) */                                       \
  typedef T Item(List(T));                                               \
  typedef struct Itr(List(T)) Itr(List(T));                              \
  struct Itr(List(T)) {                                                  \
    List(T) xs;                                                          \
  };                                                                     \
  trait_Itr(List(T));                                                    \
  /* ---- instance Iterable(List(T)) */                                  \
  trait_Iterable(List(T));                                               \
  /* ---- instance Slice(List(T)) */                                     \
  typedef struct Slice(List(T)) Slice(List(T));                          \
  struct Slice(List(T)) {                                                \
    size_t length;                                                       \
    List(T) c;                                                           \
  };                                                                     \
  trait_Slice(List(T));                                                  \
  /* ---- instance Itr(Slice(List(T))) */                                \
  typedef T Item(Slice(List(T)));                                        \
  typedef struct Itr(Slice(List(T))) Itr(Slice(List(T)));                \
  struct Itr(Slice(List(T))) {                                           \
    size_t rest;                                                         \
    Itr(List(T)) it;                                                     \
  };                                                                     \
  trait_Itr(Slice(List(T)));                                             \
  /* ---- instance Iterable(Slice(List(T))) */                           \
  trait_Iterable(Slice(List(T)));                                        \
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
  void FUNC_NAME(free, List(T))(List(T) * pxs) {                         \
    if (pxs) {                                                           \
      List(T) xs = *pxs;                                                 \
      while (xs) {                                                       \
        List(T) ys = xs;                                                 \
        xs = xs->tail;                                                   \
        *ys = (stList(T)){0};                                            \
        trait(Mem(stList(T))).free(ys);                                  \
      }                                                                  \
      *pxs = (List(T)){0};                                               \
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
  /* ---- instance Itr(List(T))*/                                        \
  static T* FUNC_NAME(ptr, Itr(List(T)))(Itr(List(T)) it) {              \
    return (it.xs ? &(it.xs->head) : 0);                                 \
  }                                                                      \
  static Itr(List(T)) FUNC_NAME(next, Itr(List(T)))(Itr(List(T)) it) {   \
    assert(it.xs);                                                       \
    it.xs = it.xs->tail;                                                 \
    return it;                                                           \
  }                                                                      \
  instance_Itr(List(T), FUNC_NAME(ptr, Itr(List(T))),                    \
               FUNC_NAME(next, Itr(List(T))));                           \
  /* ---- instance Iterable(List(T))*/                                   \
  static Itr(List(T)) FUNC_NAME(itr, Iterable(List(T)))(List(T) xs) {    \
    return (Itr(List(T))){.xs = xs};                                     \
  }                                                                      \
  instance_Iterable(List(T), FUNC_NAME(itr, Iterable(List(T))));         \
  /* ---- instance Slice(List(T)) */                                     \
  static bool FUNC_NAME(null, Slice(List(T)))(Slice(List(T)) s) {        \
    return !s.length;                                                    \
  }                                                                      \
  static size_t FUNC_NAME(length, Slice(List(T)))(Slice(List(T)) s) {    \
    return s.length;                                                     \
  }                                                                      \
  static void FUNC_NAME(reverse, Slice(List(T)))(Slice(List(T)) * ps) {  \
    assert(ps);                                                          \
    size_t len = ps->length;                                             \
    if (len <= 1) {                                                      \
      return;                                                            \
    }                                                                    \
    if (len == 2) {                                                      \
      T x = ps->c->head;                                                 \
      ps->c->head = ps->c->tail->head;                                   \
      ps->c->tail->head = x;                                             \
      return;                                                            \
    }                                                                    \
    List(T) xs = ps->c;                                                  \
    List(T) ys = xs;                                                     \
    for (size_t i = 0; i < len - 2; i++) {                               \
      ys = ys->tail;                                                     \
    }                                                                    \
    List(T) zs = ys->tail;                                               \
    ys->tail = NULL;                                                     \
    ys = xs->tail;                                                       \
    FUNC_NAME(reverse, List(T))(&xs->tail);                              \
    ys->tail = zs;                                                       \
    T x = xs->head;                                                      \
    xs->head = zs->head;                                                 \
    zs->head = x;                                                        \
    ps->c = xs;                                                          \
  }                                                                      \
  static Slice(List(T))                                                  \
      FUNC_NAME(slice, Slice(List(T)))(List(T) c, int start, int stop) { \
    if (!c) {                                                            \
      return (Slice(List(T))){0};                                        \
    }                                                                    \
    size_t len = FUNC_NAME(length, List(T))(c);                          \
    size_t s1 = adjust_index(start, len);                                \
    size_t s2 = adjust_index(stop, len);                                 \
    if (s1 >= s2) {                                                      \
      return (Slice(List(T))){0};                                        \
    }                                                                    \
    for (size_t i = 0; i < s1; i++) {                                    \
      c = c->tail;                                                       \
    }                                                                    \
    return (Slice(List(T))){.length = s2 - s1, .c = c};                  \
  }                                                                      \
  SliceT(List(T)) Trait(Slice(List(T))) {                                \
    return (SliceT(List(T))){                                            \
        .empty = {0},                                                    \
        .null = FUNC_NAME(null, Slice(List(T))),                         \
        .length = FUNC_NAME(length, Slice(List(T))),                     \
        .reverse = FUNC_NAME(reverse, Slice(List(T))),                   \
        .slice = FUNC_NAME(slice, Slice(List(T))),                       \
    };                                                                   \
  }                                                                      \
  /* ---- instance Itr(Slice(List(T)))*/                                 \
  static T* FUNC_NAME(ptr,                                               \
                      Itr(Slice(List(T))))(Itr(Slice(List(T))) it) {     \
    return (it.rest ? FUNC_NAME(ptr, Itr(List(T)))(it.it) : 0);          \
  }                                                                      \
  static Itr(Slice(List(T)))                                             \
      FUNC_NAME(next, Itr(Slice(List(T))))(Itr(Slice(List(T))) it) {     \
    assert(it.rest && !FUNC_NAME(null, Itr(List(T)))(it.it));            \
    it.rest--;                                                           \
    it.it = FUNC_NAME(next, Itr(List(T)))(it.it);                        \
    return it;                                                           \
  }                                                                      \
  instance_Itr(Slice(List(T)), FUNC_NAME(ptr, Itr(Slice(List(T)))),      \
               FUNC_NAME(next, Itr(Slice(List(T)))));                    \
  /* ---- instance Iterable(Slice(List(T)))*/                            \
  static Itr(Slice(List(T)))                                             \
      FUNC_NAME(itr, Iterable(Slice(List(T))))(Slice(List(T)) s) {       \
    return (Itr(Slice(List(T)))){                                        \
        .rest = s.length,                                                \
        .it = FUNC_NAME(itr, Iterable(List(T)))(s.c),                    \
    };                                                                   \
  }                                                                      \
  instance_Iterable(Slice(List(T)),                                      \
                    FUNC_NAME(itr, Iterable(Slice(List(T)))));           \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
