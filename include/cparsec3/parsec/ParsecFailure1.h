/* -*- coding: utf-8-unix -*- */
#include "../base/base_generics.h"

#include "ParsecPrim.h"

// -----------------------------------------------------------------------
#define ParsecFailure1(...) TYPE_NAME(ParsecFailure1, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecFailure1(S)                                          \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ParsecFailure1(S) ParsecFailure1(S);                    \
  struct ParsecFailure1(S) {                                             \
    Parsec(S, None) (*registerParseError)(ParseError(S) err);            \
    Parsec(S, None) (*registerFailure)(                                  \
        Maybe(ErrorItem(Token(S))) unexpected,                           \
        List(ErrorItem(Token(S))) expected);                             \
  };                                                                     \
                                                                         \
  ParsecFailure1(S) Trait(ParsecFailure1(S));                            \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecFailure1(S)                                           \
  C_API_BEGIN                                                            \
                                                                         \
  ParsecFailure1(S) Trait(ParsecFailure1(S)) {                           \
    return (ParsecFailure1(S)){                                          \
        .registerParseError = 0,                                         \
        .registerFailure = 0,                                            \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
