/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#define ErrorItem(S) TYPE_NAME(ErrorItem, S)
#define typedef_ErrorItem(S)                                             \
  typedef_List(Token(S));                                                \
  typedef struct {                                                       \
    enum {                                                               \
      LABEL,                                                             \
      TOKENS,                                                            \
      END_OF_INPUT,                                                      \
    } type;                                                              \
    union {                                                              \
      String label;                                                      \
      List(Token(S)) tokens;                                             \
    };                                                                   \
  } ErrorItem(S)

#define ParseError(S) TYPE_NAME(ParseError, S)
#define typedef_ParseError(S)                                            \
  typedef_ErrorItem(S);                                                  \
  typedef_Maybe(ErrorItem(S));                                           \
  typedef_List(ErrorItem(S));                                            \
  typedef struct {                                                       \
    Offset offset;                                                       \
    Maybe(ErrorItem(S)) unexpected;                                      \
    List(ErrorItem(S)) expecting;                                        \
  } ParseError(S)

// type alias for expecting items
#define Hints(S) List(ErrorItem(S))

#define ParseErrorBundle(S) TYPE_NAME(ParseErrorBundle, S)
#define typedef_ParseErrorBundle(S)                                      \
  typedef_ParseError(S);                                                 \
  typedef_List(ParseError(S));                                           \
  /* typedef_PosState(S); */                                             \
  typedef struct {                                                       \
    List(ParseError(S)) errors;                                          \
    PosState(S) posState;                                                \
  } ParseErrorBundle(S)
