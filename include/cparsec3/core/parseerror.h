/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#define ErrorItem(S) CONCAT(ErrorItem, S)
#define typedef_ErrorItem(S)                                             \
  typedef_LList(Token(S));                                               \
  typedef struct {                                                       \
    enum {                                                               \
      LABEL,                                                             \
      TOKENS,                                                            \
      END_OF_INPUT,                                                      \
    } type;                                                              \
    union {                                                              \
      String label;                                                      \
      LList(Token(S)) tokens;                                            \
    };                                                                   \
  } ErrorItem(S)

#define ParseError(S) CONCAT(ParseError, S)
#define typedef_ParseError(S)                                            \
  typedef_ErrorItem(S);                                                  \
  typedef_Maybe(ErrorItem(S));                                           \
  typedef_LList(ErrorItem(S));                                           \
  typedef struct {                                                       \
    Offset offset;                                                       \
    Maybe(ErrorItem(S)) unexpected;                                      \
    LList(ErrorItem(S)) expecting;                                       \
  } ParseError(S)

// type alias for expecting items
#define Hints(S) LList(ErrorItem(S))

#define ParseErrorBundle(S) CONCAT(ParseErrorBundle, S)
#define typedef_ParseErrorBundle(S)                                      \
  typedef_ParseError(S);                                                 \
  typedef_LList(ParseError(S));                                          \
  /* typedef_PosState(S); */                                             \
  typedef struct {                                                       \
    LList(ParseError(S)) errors;                                         \
    PosState(S) posState;                                                \
  } ParseErrorBundle(S)
