/* -*- coding: utf-8-unix -*- */
#pragma once

#include "ParsecPrim.h"

// -----------------------------------------------------------------------
#define ParsecFailure(...) TYPE_NAME(ParsecFailure, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecFailure(S, T)                                        \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ParsecFailure(S, T) ParsecFailure(S, T);                \
  struct ParsecFailure(S, T) {                                           \
    Parsec(S, T) (*parseError)(ParseError(S) e);                         \
    Parsec(S, T) (*failure)(Maybe(ErrorItem(Token(S))) unexpected,       \
                            List(ErrorItem(Token(S))) expected);         \
    Parsec(S, T) (*unexpected)(ErrorItem(Token(S)) item);                \
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
  impl_parseError(S, T);                                                 \
                                                                         \
  ParsecFailure(S, T) Trait(ParsecFailure(S, T)) {                       \
    return (ParsecFailure(S, T)){                                        \
        .parseError = FUNC_NAME(parseError, S, T),                       \
        .failure = 0,                                                    \
        .unexpected = 0,                                                 \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* parseError(e) */
#define impl_parseError(S, T)                                            \
  typedef_Fn_r(ParseError(S), UnParser(S, T));                           \
  fn(FUNC_NAME(parseErrorImpl, S, T), ParseError(S),                     \
     UnParserArgs(S, T)) {                                               \
    g_bind((e, s, , , , eerr), *args);                                   \
    return fn_apply(eerr, e, s);                                         \
  }                                                                      \
                                                                         \
  static Parsec(S, T) FUNC_NAME(parseError, S, T)(ParseError(S) e) {     \
    __auto_type f = FUNC_NAME(parseErrorImpl, S, T)();                   \
    return (Parsec(S, T)){.unParser = fn_apply(f, e)};                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
