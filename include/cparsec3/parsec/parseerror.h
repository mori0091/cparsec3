/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#define ErrorItem(T) TYPE_NAME(ErrorItem, T)
#define typedef_ErrorItem(T)                                             \
  C_API_BEGIN                                                            \
  typedef struct ErrorItem(T) ErrorItem(T);                              \
  struct ErrorItem(T) {                                                  \
    enum {                                                               \
      LABEL,                                                             \
      TOKENS,                                                            \
      END_OF_INPUT,                                                      \
    } type;                                                              \
    union {                                                              \
      String label;                                                      \
      List(T) tokens;                                                    \
    };                                                                   \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define trait_ErrorItem(T)                                               \
  C_API_BEGIN                                                            \
  typedef_ErrorItem(T);                                                  \
  trait_Eq(ErrorItem(T));                                                \
  trait_Ord(ErrorItem(T));                                               \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define ParseError(S) TYPE_NAME(ParseError, S)
#define typedef_ParseError(S)                                            \
  C_API_BEGIN                                                            \
  trait_ErrorItem(Token(S));                                             \
  trait_Maybe(ErrorItem(Token(S)));                                      \
  typedef ErrorItem(Token(S)) Item(List(ErrorItem(Token(S))));           \
  trait_List(ErrorItem(Token(S)));                                       \
  typedef struct ParseError(S) ParseError(S);                            \
  struct ParseError(S) {                                                 \
    Offset offset;                                                       \
    Maybe(ErrorItem(Token(S))) unexpected;                               \
    List(ErrorItem(Token(S))) expecting;                                 \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define trait_ParseError(S)                                              \
  C_API_BEGIN                                                            \
  typedef_ParseError(S);                                                 \
  trait_Eq(ParseError(S));                                               \
  trait_Ord(ParseError(S));                                              \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define impl_ParseError(S)                                               \
  C_API_BEGIN                                                            \
  impl_Maybe(ErrorItem(Token(S)));                                       \
  impl_List(ErrorItem(Token(S)));                                        \
  C_API_END                                                              \
  END_OF_STATEMENTS

// type alias for expecting items
#define Hints(T) List(ErrorItem(T))

#define ParseErrorBundle(S) TYPE_NAME(ParseErrorBundle, S)
#define typedef_ParseErrorBundle(S)                                      \
  C_API_BEGIN                                                            \
  typedef struct ParseErrorBundle(S) ParseErrorBundle(S);                \
  struct ParseErrorBundle(S) {                                           \
    List(ParseError(S)) errors;                                          \
    PosState(S) posState;                                                \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

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
