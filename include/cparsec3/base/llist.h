/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define LList_CONS(T) FUNC_NAME(llist_cons, T)
#define LList_DROP(T) FUNC_NAME(llist_drop, T)
#define LList(T) TYPE_NAME(LList, T)
#define LListSt(T) TYPE_NAME(LListSt, T)
#define typedef_LList(T)                                                 \
  C_API_BEGIN                                                            \
  typedef struct LListSt(T) * LList(T);                                  \
  struct LListSt(T) {                                                    \
    LList(T) tail;                                                       \
    T head;                                                              \
  };                                                                     \
  static inline LList(T) LList_CONS(T)(T x, LList(T) xs) {               \
    LList(T) ys = (LList(T))malloc(sizeof(struct LListSt(T)));           \
    ys->head = x;                                                        \
    ys->tail = xs;                                                       \
    return ys;                                                           \
  }                                                                      \
  static inline void LList_DROP(T)(LList(T) xs, void (*dropT)(T*)) {     \
    while (xs) {                                                         \
      LList(T) ys = xs;                                                  \
      xs = xs->tail;                                                     \
      if (dropT) {                                                       \
        dropT(&(ys->head));                                              \
      }                                                                  \
      *ys = (struct LListSt(T)){0};                                      \
      free(ys);                                                          \
    }                                                                    \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS
