/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#include "sourcepos.h"

#define PosState(S) TYPE_NAME(PosState, S)
#define typedef_PosState(S)                                              \
  typedef struct {                                                       \
    S input; /* a Stream */                                              \
    Offset offset;                                                       \
    SourcePos pos;                                                       \
    size_t tabWidth;                                                     \
    String linePrefix;                                                   \
  } PosState(S);                                                         \
  static inline PosState(S) initialPosState(String name, S input) {      \
    return (PosState(S)){.input = input,                                 \
                         .offset = 0,                                    \
                         .pos = initialSourcePos(name),                  \
                         .tabWidth = 8,                                  \
                         .linePrefix = ""};                              \
  }                                                                      \
  END_OF_STATEMENTS
