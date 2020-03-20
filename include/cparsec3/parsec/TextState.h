/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

// -----------------------------------------------------------------------
#define TextState(S) TYPE_NAME(TextState, S)
#define TextStateT(S) TYPE_NAME(TextStateT, S)

// -----------------------------------------------------------------------
#define typedef_TextState(S)                                             \
  typedef struct TextState(S) TextState(S);                              \
  struct TextState(S) {                                                  \
    S input;      /**< rest of the unprocessed token-stream */           \
    S headOfLine; /**< head of line */                                   \
    size_t line;  /**< line number */                                    \
  };                                                                     \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define trait_TextState(S)                                               \
  typedef_TextState(S);                                                  \
                                                                         \
  typedef struct TextStateT(S) TextStateT(S);                            \
  struct TextStateT(S) {                                                 \
    TextState(S) (*create)(S input);                                     \
  };                                                                     \
                                                                         \
  TextStateT(S) Trait(TextState(S));                                     \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_TextState(S)                                                \
  static inline TextState(S) FUNC_NAME(create, TextState(S))(S input) {  \
    return (TextState(S)){                                               \
        .input = input,                                                  \
        .headOfLine = input,                                             \
        .line = 1,                                                       \
    };                                                                   \
  }                                                                      \
                                                                         \
  TextStateT(S) Trait(TextState(S)) {                                    \
    return (TextStateT(S)){                                              \
        .create = FUNC_NAME(create, TextState(S)),                       \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
trait_TextState(String);
