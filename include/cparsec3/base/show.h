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
  static inline String FUNC_NAME(show, Show(T))(T x) {                   \
    char* s;                                                             \
    assert(0 <= mem_asprintf(&s, fmt, x));                               \
    return s;                                                            \
  }                                                                      \
                                                                         \
  instance_Show(T, FUNC_NAME(show, Show(T)));                            \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Show(T, _show_)                                         \
  C_API_BEGIN                                                            \
                                                                         \
  Show(T) Trait(Show(T)) {                                               \
    return (Show(T)){                                                    \
        .show = _show_,                                                  \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
