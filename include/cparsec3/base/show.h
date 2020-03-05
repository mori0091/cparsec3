/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "mem.h"

C_API_BEGIN
void quote_char(CharBuff* b, char x);
C_API_END

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

// -----------------------------------------------------------------------
#define impl_ShowSeq(C, T)                                               \
  C_API_BEGIN                                                            \
                                                                         \
  static inline void FUNC_NAME(toString, Show(C(T)))(CharBuff * b,       \
                                                     C(T) c) {           \
    if (trait(C(T)).null(c)) {                                           \
      mem_printf(b, "[]");                                               \
      return;                                                            \
    }                                                                    \
                                                                         \
    Show(T) s = trait(Show(T));                                          \
    ItrT(C(T)) I = trait(Itr(C(T)));                                     \
                                                                         \
    Itr(C(T)) it = trait(Iterable(C(T))).itr(c);                         \
    mem_printf(b, "[");                                                  \
    s.toString(b, I.get(it));                                            \
    it = I.next(it);                                                     \
    while (!I.null(it)) {                                                \
      mem_printf(b, ", ");                                               \
      s.toString(b, I.get(it));                                          \
      it = I.next(it);                                                   \
    }                                                                    \
    mem_printf(b, "]");                                                  \
  }                                                                      \
                                                                         \
  instance_Show(C(T), FUNC_NAME(toString, Show(C(T))));                  \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
