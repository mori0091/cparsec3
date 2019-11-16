/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#define ErrorItem(S) TYPE_NAME(ErrorItem, S)
#define typedef_ErrorItem(S)                                             \
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

#define trait_ErrorItem(S)                                               \
  C_API_BEGIN                                                            \
  typedef_ErrorItem(S);                                                  \
  trait_Eq(ErrorItem(S));                                                \
  trait_Ord(ErrorItem(S));                                               \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define ParseError(S) TYPE_NAME(ParseError, S)
#define typedef_ParseError(S)                                            \
  trait_ErrorItem(S);                                                    \
  trait_Maybe(ErrorItem(S));                                             \
  trait_List(ErrorItem(S));                                              \
  typedef struct {                                                       \
    Offset offset;                                                       \
    Maybe(ErrorItem(S)) unexpected;                                      \
    List(ErrorItem(S)) expecting;                                        \
  } ParseError(S)

#define trait_ParseError(S)                                              \
  C_API_BEGIN                                                            \
  typedef_ParseError(S);                                                 \
  trait_Eq(ParseError(S));                                               \
  trait_Ord(ParseError(S));                                              \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define impl_ParseError(S)                                               \
  C_API_BEGIN                                                            \
  impl_Maybe(ErrorItem(S));                                              \
  impl_List(ErrorItem(S));                                               \
  C_API_END                                                              \
  END_OF_STATEMENTS

// type alias for expecting items
#define Hints(S) List(ErrorItem(S))

#define ParseErrorBundle(S) TYPE_NAME(ParseErrorBundle, S)
#define typedef_ParseErrorBundle(S)                                      \
  typedef struct {                                                       \
    List(ParseError(S)) errors;                                          \
    PosState(S) posState;                                                \
  } ParseErrorBundle(S)

#define trait_ParseErrorBundle(S)                                        \
  C_API_BEGIN                                                            \
  trait_ParseError(S);                                                   \
  trait_List(ParseError(S));                                             \
  typedef_ParseErrorBundle(S);                                           \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define impl_ParseErrorBundle(S)                                         \
  C_API_BEGIN                                                            \
  impl_List(ParseError(S));                                              \
  C_API_END                                                              \
  END_OF_STATEMENTS
