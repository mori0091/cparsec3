/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

// -----------------------------------------------------------------------
#define IndexState(S) TYPE_NAME(IndexState, S)
#define IndexStateT(S) TYPE_NAME(IndexStateT, S)

// -----------------------------------------------------------------------
#define typedef_IndexState(S)                                            \
  typedef struct IndexState(S) IndexState(S);                            \
  struct IndexState(S) {                                                 \
    S input;      /**< rest of the unprocessed token-stream */           \
    size_t index; /**< index begining with 0 */                          \
  };                                                                     \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define trait_IndexState(S)                                              \
  typedef_IndexState(S);                                                 \
                                                                         \
  typedef struct IndexStateT(S) IndexStateT(S);                          \
  struct IndexStateT(S) {                                                \
    IndexState(S) (*create)(S input);                                    \
  };                                                                     \
                                                                         \
  IndexStateT(S) Trait(IndexState(S));                                   \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_IndexState(S)                                               \
  static inline IndexState(S)                                            \
      FUNC_NAME(create, IndexState(S))(S input) {                        \
    return (IndexState(S)){                                              \
        .input = input,                                                  \
        .index = 0,                                                      \
    };                                                                   \
  }                                                                      \
                                                                         \
  IndexStateT(S) Trait(IndexState(S)) {                                  \
    return (IndexStateT(S)){                                             \
        .create = FUNC_NAME(create, IndexState(S)),                      \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
trait_IndexState(String);
