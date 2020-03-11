/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#include "sourcepos.h"

// -----------------------------------------------------------------------
#define State(S) TYPE_NAME(State, S)
#define StateT(S) TYPE_NAME(StateT, S)

// -----------------------------------------------------------------------
#define typedef_State(S)                                                 \
  typedef struct State(S) State(S);                                      \
  struct State(S) {                                                      \
    S input;             /**< rest of the unprocessed token-stream */    \
    Offset offset;       /**< offset of the current position */          \
    Offset lineOffset;   /**< offset of the current line */              \
    SourcePos sourcePos; /**< line/column number */                      \
    size_t tabWidth;     /**< TAB width for calculation of column */     \
    String linePrefix;                                                   \
  };                                                                     \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define trait_State(S)                                                   \
  typedef_State(S);                                                      \
                                                                         \
  typedef struct StateT(S) StateT(S);                                    \
  struct StateT(S) {                                                     \
    State(S) (*create)(S input);                                         \
  };                                                                     \
                                                                         \
  StateT(S) Trait(State(S));                                             \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_State(S)                                                    \
  static inline State(S) FUNC_NAME(create, State(S))(S input) {          \
    return (State(S)){                                                   \
        .input = input,                                                  \
        .offset = 0,                                                     \
        .lineOffset = 0,                                                 \
        .sourcePos.line = 1,                                             \
        .sourcePos.column = 1,                                           \
        .tabWidth = 8,                                                   \
        .linePrefix = " | ",                                             \
    };                                                                   \
  }                                                                      \
                                                                         \
  StateT(S) Trait(State(S)) {                                            \
    return (StateT(S)){                                                  \
        .create = FUNC_NAME(create, State(S)),                           \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
trait_State(String);
