/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#define Mem(T) TYPE_NAME(Mem, T)
#define MemT(T) TYPE_NAME(MemT, T)
// -----------------------------------------------------------------------
#define trait_Mem(T)                                                     \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    T* (*create)(size_t);                                                \
    T* (*recreate)(T*, size_t);                                          \
    void (*free)(T*);                                                    \
  } MemT(T);                                                             \
  MemT(T) Trait(Mem(T));                                                 \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Mem(T)                                                      \
  C_API_BEGIN                                                            \
  static T* FUNC_NAME(create, Mem(T))(size_t n) {                        \
    return (T*)malloc(n * sizeof(T));                                    \
  }                                                                      \
  static T* FUNC_NAME(recreate, Mem(T))(T * ptr, size_t n) {             \
    return (T*)realloc(ptr, n * sizeof(T));                              \
  }                                                                      \
  static void FUNC_NAME(free, Mem(T))(T * p) {                           \
    free((void*)p);                                                      \
  }                                                                      \
  MemT(T) Trait(Mem(T)) {                                                \
    return (MemT(T)){                                                    \
        .create = FUNC_NAME(create, Mem(T)),                             \
        .recreate = FUNC_NAME(recreate, Mem(T)),                         \
        .free = FUNC_NAME(free, Mem(T)),                                 \
    };                                                                   \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
// FOREACH(trait_Mem, TYPESET(ALL));
