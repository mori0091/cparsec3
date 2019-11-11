/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Array(T) TYPE_NAME(Array, T)
#define ArrayT(T) TYPE_NAME(ArrayT, T)
// -----------------------------------------------------------------------
#define typedef_Array(T)                                                 \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    size_t length;                                                       \
    T* data;                                                             \
  } Array(T);                                                            \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define trait_Array(T)                                                   \
  C_API_BEGIN                                                            \
  typedef_Array(T);                                                      \
  typedef struct {                                                       \
    Array(T) empty;                                                      \
    bool (*null)(Array(T) a);                                            \
    size_t (*length)(Array(T) a);                                        \
    Array(T) (*create)(size_t n);                                        \
    void (*free)(Array(T) a);                                            \
    T* (*begin)(Array(T) a);                                             \
    T* (*end)(Array(T) a);                                               \
  } ArrayT(T);                                                           \
  ArrayT(T) Trait(Array(T));                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Array(T)                                                    \
  C_API_BEGIN                                                            \
  static bool FUNC_NAME(null, Array(T))(Array(T) a) {                    \
    return !a.length;                                                    \
  }                                                                      \
  static size_t FUNC_NAME(length, Array(T))(Array(T) a) {                \
    return a.length;                                                     \
  }                                                                      \
  static Array(T) FUNC_NAME(create, Array(T))(size_t n) {                \
    return (Array(T)){.length = n, .data = trait(Mem(T)).create(n)};     \
  }                                                                      \
  static void FUNC_NAME(free, Array(T))(Array(T) a) {                    \
    trait(Mem(T)).free(a.data);                                          \
  }                                                                      \
  static T* FUNC_NAME(begin, Array(T))(Array(T) a) {                     \
    return a.data;                                                       \
  }                                                                      \
  static T* FUNC_NAME(end, Array(T))(Array(T) a) {                       \
    return a.data + a.length;                                            \
  }                                                                      \
  ArrayT(T) Trait(Array(T)) {                                            \
    return (ArrayT(T)){                                                  \
        .empty = {0},                                                    \
        .null = FUNC_NAME(null, Array(T)),                               \
        .length = FUNC_NAME(length, Array(T)),                           \
        .create = FUNC_NAME(create, Array(T)),                           \
        .free = FUNC_NAME(free, Array(T)),                               \
        .begin = FUNC_NAME(begin, Array(T)),                             \
        .end = FUNC_NAME(end, Array(T)),                                 \
    };                                                                   \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS
