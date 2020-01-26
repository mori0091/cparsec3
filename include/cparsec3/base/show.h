/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "mem.h"

// -----------------------------------------------------------------------
#define Show(T) TYPE_NAME(Show, T)

// -----------------------------------------------------------------------
#define trait_Show(T)                                                    \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct Show(T) Show(T);                                        \
  struct Show(T) {                                                       \
    void (*toString)(CharBuff * b, T x);                                 \
    String (*show)(T x);                                                 \
  };                                                                     \
                                                                         \
  Show(T) Trait(Show(T));                                                \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Show(T, fmt)                                                \
  C_API_BEGIN                                                            \
                                                                         \
  static inline void FUNC_NAME(toString, Show(T))(CharBuff * b, T x) {   \
    assert(0 <= mem_printf(b, fmt, x));                                  \
  }                                                                      \
                                                                         \
  instance_Show(T, FUNC_NAME(toString, Show(T)));                        \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Show(T, _toString_)                                     \
  C_API_BEGIN                                                            \
                                                                         \
  static inline String FUNC_NAME(show, Show(T))(T x) {                   \
    CharBuff b = {0};                                                    \
    _toString_(&b, x);                                                   \
    return b.data;                                                       \
  }                                                                      \
                                                                         \
  Show(T) Trait(Show(T)) {                                               \
    return (Show(T)){                                                    \
        .toString = _toString_,                                          \
        .show = FUNC_NAME(show, Show(T)),                                \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
