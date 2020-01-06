/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#include "posstate.h"

#define ParseState(S) TYPE_NAME(ParseState, S)
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
  static inline ParseState(S) initialParseState(String name, S input) {  \
    return (ParseState(S)){.input = input,                               \
                           .offset = 0,                                  \
                           .posState = initialPosState(name, input)};    \
  }                                                                      \
  END_OF_STATEMENTS
