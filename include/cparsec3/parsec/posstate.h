/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#include "sourcepos.h"

#define PosState(S) TYPE_NAME(PosState, S)
#define PosStateT(S) TYPE_NAME(PosStateT, S)

#define typedef_PosState(S)                                              \
  typedef struct PosState(S) PosState(S);                                \
  struct PosState(S) {                                                   \
    S input;           /**< rest of the unprocessed token-stream */      \
    Offset offset;     /**< offset of the current position */            \
    Offset lineOffset; /**< offset of the current line */                \
    SourcePos sourcePos;                                                 \
    size_t tabWidth;                                                     \
    String linePrefix;                                                   \
  };                                                                     \
                                                                         \
  END_OF_STATEMENTS

#define trait_PosState(S)                                                \
  typedef_PosState(S);                                                   \
                                                                         \
  typedef struct PosStateT(S) PosStateT(S);                              \
  struct PosStateT(S) {                                                  \
    PosState(S) (*create)(S input);                                      \
    void (*print)(String lineText, PosState(S) s);                       \
  };                                                                     \
                                                                         \
  PosStateT(S) Trait(PosState(S));                                       \
                                                                         \
  END_OF_STATEMENTS

#define impl_PosState(S)                                                 \
  static inline PosState(S) FUNC_NAME(create, PosState(S))(S input) {    \
    return (PosState(S)){                                                \
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
  static inline void FUNC_NAME(print, PosState(S))(String lineText,      \
                                                   PosState(S) s) {      \
    int line = s.sourcePos.line;                                         \
    int column = s.sourcePos.column;                                     \
    String linePrefix = s.linePrefix;                                    \
    assert(1 <= line);                                                   \
    assert(1 <= column);                                                 \
    int n = snprintf(0, 0, "%d", line);                                  \
    assert(1 <= n);                                                      \
    printf("%d:%d:\n", line, column);                                    \
    printf("%*s%s\n", n, "", linePrefix);                                \
    printf("%*d%s%s\n", n, line, linePrefix, lineText);                  \
    printf("%*s%s%*s^\n", n, "", linePrefix, column - 1, "");            \
  }                                                                      \
                                                                         \
  PosStateT(S) Trait(PosState(S)) {                                      \
    return (PosStateT(S)){                                               \
        .create = FUNC_NAME(create, PosState(S)),                        \
        .print = FUNC_NAME(print, PosState(S)),                          \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
