/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#include "posstate.h"

#define ParseState(S) TYPE_NAME(ParseState, S)
#define ParseStateT(S) TYPE_NAME(ParseStateT, S)

#define typedef_ParseState(S)                                            \
  typedef struct ParseState(S) ParseState(S);                            \
  struct ParseState(S) {                                                 \
    /** the rest of input to process */                                  \
    S input;                                                             \
    /** number of processed tokens so far */                             \
    Offset offset;                                                       \
    /** state that is used for line/col calculation */                   \
    PosState(S) posState;                                                \
  };                                                                     \
                                                                         \
  END_OF_STATEMENTS

#define trait_ParseState(S)                                              \
  typedef_ParseState(S);                                                 \
                                                                         \
  typedef struct ParseStateT(S) ParseStateT(S);                          \
  struct ParseStateT(S) {                                                \
    ParseState(S) (*create)(String name, S input);                       \
  };                                                                     \
                                                                         \
  ParseStateT(S) Trait(ParseState(S));                                   \
                                                                         \
  END_OF_STATEMENTS

#define impl_ParseState(S)                                               \
  static inline ParseState(S)                                            \
      FUNC_NAME(create, ParseState(S))(String name, S input) {           \
    return (ParseState(S)){                                              \
        .input = input,                                                  \
        .offset = 0,                                                     \
        .posState = trait(PosState(S)).create(name, input),              \
    };                                                                   \
  }                                                                      \
                                                                         \
  ParseStateT(S) Trait(ParseState(S)) {                                  \
    return (ParseStateT(S)){                                             \
        .create = FUNC_NAME(create, ParseState(S)),                      \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
