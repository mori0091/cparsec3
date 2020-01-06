/* -*- coding: utf-8-unix -*- */
#include "../base/base_generics.h"

#include "ParsecPrim.h"

// -----------------------------------------------------------------------
#define ParsecFailure(...) TYPE_NAME(ParsecFailure, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecFailure(S, T)                                        \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ParsecFailure(S, T) ParsecFailure(S, T);                \
  struct ParsecFailure(S, T) {                                           \
    Parsec(S, T) (*failure)(Maybe(ErrorItem(Token(S))) unexpected,       \
                            List(ErrorItem(Token(S))) expected);         \
    Parsec(S, T) (*unexpected)(ErrorItem(Token(S)) item);                \
    Parsec(S, None) (*registerParseError)(ParseError(S) err);            \
    Parsec(S, None) (*registerFailure)(                                  \
        Maybe(ErrorItem(Token(S))) unexpected,                           \
        List(ErrorItem(Token(S))) expected);                             \
  };                                                                     \
                                                                         \
  ParsecFailure(S, T) Trait(ParsecFailure(S, T));                        \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecFailure(S, T)                                         \
  C_API_BEGIN                                                            \
                                                                         \
  ParsecFailure(S, T) Trait(ParsecFailure(S, T)) {                       \
    return (ParsecFailure(S, T)){                                        \
        .failure = 0,                                                    \
        .unexpected = 0,                                                 \
        .registerParseError = 0,                                         \
        .registerFailure = 0,                                            \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
