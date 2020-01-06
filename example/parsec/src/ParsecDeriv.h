/* -*- coding: utf-8-unix -*- */
#include <cparsec3/base/base_generics.h>

#include "ParsecPrim.h"

// -----------------------------------------------------------------------
#define ParsecDeriv(S) TYPE_NAME(ParsecDeriv, S)

// -----------------------------------------------------------------------
#define trait_ParsecDeriv(S)                                             \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ParsecDeriv(S) ParsecDeriv(S);                          \
  struct ParsecDeriv(S) {                                                \
    Parsec(S, Token(S)) (*single)(Token(S) t);                           \
    Parsec(S, Token(S)) (*satisfy)(Fn(Token(S), bool) pred);             \
    Parsec(S, Token(S)) (*anySingle)(void);                              \
    Parsec(S, Token(S)) (*anySingleBut)(Token(S) t);                     \
    Parsec(S, Token(S)) (*oneOf)(Array(Token(S)) ts);                    \
    Parsec(S, Token(S)) (*noneOf)(Array(Token(S)) ts);                   \
    Parsec(S, Tokens(S)) (*chunk)(Tokens(S) chk);                        \
    Parsec(S, Tokens(S)) (*takeRest)(void);                              \
  };                                                                     \
                                                                         \
  ParsecDeriv(S) Trait(ParsecDeriv(S));                                  \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecDeriv(S)                                              \
  C_API_BEGIN                                                            \
                                                                         \
  impl_single(S);                                                        \
  impl_satisfy(S);                                                       \
  impl_anySingle(S);                                                     \
  impl_anySingleBut(S);                                                  \
                                                                         \
  ParsecDeriv(S) Trait(ParsecDeriv(S)) {                                 \
    return (ParsecDeriv(S)){                                             \
        .single = FUNC_NAME(single, S),                                  \
        .satisfy = FUNC_NAME(satisfy, S),                                \
        .anySingle = FUNC_NAME(anySingle, S),                            \
        .anySingleBut = FUNC_NAME(anySingleBut, S),                      \
        .oneOf = 0,                                                      \
        .noneOf = 0,                                                     \
        .chunk = 0,                                                      \
        .takeRest = 0,                                                   \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* single(t) */
#define impl_single(S)                                                   \
  typedef_Fn_r(Token(S), Fn(Token(S), Maybe(Token(S))));                 \
                                                                         \
  fn(FUNC_NAME(singleTestToken, S), Token(S), Token(S),                  \
     Maybe(Token(S))) {                                                  \
    g_bind((expect, actual), *args);                                     \
    if (trait(Eq(Token(S))).neq(expect, actual)) {                       \
      return (Maybe(Token(S))){.none = true};                            \
    }                                                                    \
    return (Maybe(Token(S))){.value = actual};                           \
  }                                                                      \
                                                                         \
  static Parsec(S, Token(S)) FUNC_NAME(single, S)(Token(S) t) {          \
    __auto_type f = FUNC_NAME(singleTestToken, S)();                     \
    return trait(ParsecPrim(S, Token(S)))                                \
        .token(fn_apply(f, t), (Hints(Token(S))){0});                    \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* satisfy(f) */
#define impl_satisfy(S)                                                  \
  typedef_Fn_r(Fn(Token(S), bool), Fn(Token(S), Maybe(Token(S))));       \
                                                                         \
  fn(FUNC_NAME(satisfyTestToken, S), Fn(Token(S), bool), Token(S),       \
     Maybe(Token(S))) {                                                  \
    g_bind((pred, t), *args);                                            \
    if (!fn_apply(pred, t)) {                                            \
      return (Maybe(Token(S))){.none = true};                            \
    }                                                                    \
    return (Maybe(Token(S))){.value = t};                                \
  }                                                                      \
                                                                         \
  static Parsec(S, Token(S))                                             \
      FUNC_NAME(satisfy, S)(Fn(Token(S), bool) pred) {                   \
    __auto_type f = FUNC_NAME(satisfyTestToken, S)();                    \
    return trait(ParsecPrim(S, Token(S)))                                \
        .token(fn_apply(f, pred), (Hints(Token(S))){0});                 \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* anySingle() */
#define impl_anySingle(S)                                                \
                                                                         \
  fn(FUNC_NAME(anySingleTestToken, S), Token(S), bool) {                 \
    UNUSED(args);                                                        \
    return true;                                                         \
  }                                                                      \
                                                                         \
  static Parsec(S, Token(S)) FUNC_NAME(anySingle, S)(void) {             \
    __auto_type f = FUNC_NAME(anySingleTestToken, S)();                  \
    return FUNC_NAME(satisfy, S)(f);                                     \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* anySingleBut() */
#define impl_anySingleBut(S)                                             \
  typedef_Fn_r(Token(S), Fn(Token(S), bool));                            \
                                                                         \
  fn(FUNC_NAME(anySingleButTestToken, S), Token(S), Token(S), bool) {    \
    g_bind((unexpected, actual), *args);                                 \
    return trait(Eq(Token(S))).neq(unexpected, actual);                  \
  }                                                                      \
                                                                         \
  static Parsec(S, Token(S)) FUNC_NAME(anySingleBut, S)(Token(S) t) {    \
    __auto_type f = FUNC_NAME(anySingleButTestToken, S)();               \
    return FUNC_NAME(satisfy, S)(fn_apply(f, t));                        \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
