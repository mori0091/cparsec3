/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../../base/base_generics.h"

#include "../parsec.h"

// -----------------------------------------------------------------------
#define ParsecToken(...) TYPE_NAME(ParsecToken, __VA_ARGS__)

#define FnMapToken(S, T) Fn(Token(S), Maybe(T))

// -----------------------------------------------------------------------
#define trait_ParsecToken(S, T)                                          \
  C_API_BEGIN                                                            \
                                                                         \
  typedef_Fn(Token(S), Maybe(T));                                        \
                                                                         \
  typedef struct ParsecToken(S, T) ParsecToken(S, T);                    \
  struct ParsecToken(S, T) {                                             \
    Parsec(S, T) (*pSatisfyMap)(FnMapToken(S, T) testToken,              \
                                Hints(S) expecting);                     \
  };                                                                     \
                                                                         \
  ParsecToken(S, T) Trait(ParsecToken(S, T));                            \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecToken(S, T)                                           \
  C_API_BEGIN                                                            \
                                                                         \
  impl_satisfyMap(S, T);                                                 \
                                                                         \
  ParsecToken(S, T) Trait(ParsecToken(S, T)) {                           \
    return (ParsecToken(S, T)){                                          \
        .pSatisfyMap = FUNC_NAME(satisfyMap, S, T),                      \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* satisfyMap(testToken, expecting) */
#define impl_satisfyMap(S, T)                                            \
  typedef_Fn_r(Fn(Token(S), Maybe(T)), Hints(S), UnParser(S, T));        \
  fn(FUNC_NAME(satisfyMapImpl, S, T), Fn(Token(S), Maybe(T)), Hints(S),  \
     UnParserArgs(S, T)) {                                               \
    g_bind((testToken, expect, s, cok, , , eerr), *args);                \
    Stream(S) SS = trait(Stream(S));                                     \
    __auto_type maybe = SS.take1(s);                                     \
    if (maybe.none) {                                                    \
      ParseError(S) e = {                                                \
          .offset = SS.offsetOf(s),                                      \
          .unexpected.value.type = END_OF_INPUT,                         \
          .expecting = expect,                                           \
      };                                                                 \
      return fn_apply(eerr, e, s);                                       \
    }                                                                    \
                                                                         \
    Token(S) a = maybe.value.e1;                                         \
    __auto_type maybe2 = fn_apply(testToken, a);                         \
    if (maybe2.none) {                                                   \
      ParseError(S) e = {                                                \
          .offset = SS.offsetOf(s),                                      \
          .unexpected.value.type = TOKENS,                               \
          .unexpected.value.tokens =                                     \
              trait(List(Token(S))).cons(a, NULL),                       \
          .expecting = expect,                                           \
      };                                                                 \
      return fn_apply(eerr, e, s);                                       \
    }                                                                    \
                                                                         \
    return fn_apply(cok, maybe2.value, maybe.value.e2, NULL);            \
  }                                                                      \
                                                                         \
  static Parsec(S, T) FUNC_NAME(satisfyMap, S, T)(                       \
      FnMapToken(S, T) testToken, Hints(S) expecting) {                  \
    __auto_type f = FUNC_NAME(satisfyMapImpl, S, T)();                   \
    return (Parsec(S, T)){                                               \
        .unParser = fn_apply(f, testToken, expecting),                   \
    };                                                                   \
  }                                                                      \
  END_OF_STATEMENTS
