/* -*- coding: utf-8-unix -*- */
#pragma once

#include "ParsecToken.h"

// -----------------------------------------------------------------------
#define ParsecToken1(S) TYPE_NAME(ParsecToken1, S)

// -----------------------------------------------------------------------
#define trait_ParsecToken1(S)                                            \
  C_API_BEGIN                                                            \
                                                                         \
  typedef_Fn_r(Token(S), bool);                                          \
  typedef_Fn_r(Tokens(S), Tokens(S), bool);                              \
                                                                         \
  typedef struct ParsecToken1(S) ParsecToken1(S);                        \
  struct ParsecToken1(S) {                                               \
    Parsec(S, Tokens(S)) (*pTokens)(Fn(Tokens(S), Tokens(S), bool) test, \
                                    Tokens(S) pattern);                  \
    Parsec(S, Tokens(S)) (*pTakeWhileP)(Maybe(String) name,              \
                                        Fn(Token(S), bool) pred);        \
    Parsec(S, Tokens(S)) (*pTakeWhile1P)(Maybe(String) name,             \
                                         Fn(Token(S), bool) pred);       \
    Parsec(S, Tokens(S)) (*pTakeP)(Maybe(String) name, int n);           \
    Parsec(S, None) (*pEof)(void);                                       \
                                                                         \
    Parsec(S, Token(S)) (*pSingle)(Token(S) t);                          \
    Parsec(S, Token(S)) (*pSatisfy)(Fn(Token(S), bool) pred);            \
    Parsec(S, Token(S)) (*pAnySingle)(void);                             \
    Parsec(S, Token(S)) (*pAnySingleBut)(Token(S) t);                    \
    Parsec(S, Token(S)) (*pOneOf)(Array(Token(S)) ts);                   \
    Parsec(S, Token(S)) (*pNoneOf)(Array(Token(S)) ts);                  \
    Parsec(S, Tokens(S)) (*pChunk)(Tokens(S) chk);                       \
    Parsec(S, Tokens(S)) (*pTakeRest)(void);                             \
  };                                                                     \
                                                                         \
  ParsecToken1(S) Trait(ParsecToken1(S));                                \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecToken1(S)                                             \
  C_API_BEGIN                                                            \
                                                                         \
  impl_tokens(S);                                                        \
                                                                         \
  impl_single(S);                                                        \
  impl_satisfy(S);                                                       \
  impl_anySingle(S);                                                     \
  impl_anySingleBut(S);                                                  \
                                                                         \
  impl_chunk(S);                                                         \
                                                                         \
  ParsecToken1(S) Trait(ParsecToken1(S)) {                               \
    return (ParsecToken1(S)){                                            \
        .pTokens = FUNC_NAME(tokens, S),                                 \
        .pTakeWhileP = 0,                                                \
        .pTakeWhile1P = 0,                                               \
        .pTakeP = 0,                                                     \
        .pEof = 0,                                                       \
                                                                         \
        .pSingle = FUNC_NAME(single, S),                                 \
        .pSatisfy = FUNC_NAME(satisfy, S),                               \
        .pAnySingle = FUNC_NAME(anySingle, S),                           \
        .pAnySingleBut = FUNC_NAME(anySingleBut, S),                     \
        .pOneOf = 0,                                                     \
        .pNoneOf = 0,                                                    \
        .pChunk = FUNC_NAME(chunk, S),                                   \
        .pTakeRest = 0,                                                  \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* tokens(test, pattern) */
#define impl_tokens(S)                                                   \
                                                                         \
  typedef_Fn_r(Fn(Tokens(S), Tokens(S), bool), Tokens(S),                \
               UnParser(S, Tokens(S)));                                  \
                                                                         \
  fn(FUNC_NAME(tokensImpl, S),                                           \
     Fn(Tokens(S), Tokens(S), bool), /* test */                          \
     Tokens(S),                      /* pattern */                       \
     UnParserArgs(                                                       \
         S, Tokens(S)) /* s -> cok -> cerr -> eok -> eerr -> reply*/     \
  ) {                                                                    \
    g_bind((test, pattern, s, cok, , eok, eerr), *args);                 \
    Stream(S) IS = trait(Stream(S));                                     \
    int n = IS.chunkLength(pattern);                                     \
    __auto_type maybe = IS.takeN(n, s);                                  \
    if (maybe.none) {                                                    \
      ParseError(S) e = {                                                \
          .offset = IS.offsetOf(s),                                      \
          .unexpected.value.type = END_OF_INPUT,                         \
          .expecting = NULL,                                             \
      };                                                                 \
      return fn_apply(eerr, e, s);                                       \
    }                                                                    \
    Tokens(S) actual = maybe.value.e1;                                   \
    if (!fn_apply(test, pattern, actual)) {                              \
      ParseError(S) e = {                                                \
          .offset = IS.offsetOf(s),                                      \
          .unexpected.value = {.type = TOKENS,                           \
                               .tokens = IS.chunkToTokens(actual)},      \
          .expecting = NULL,                                             \
      };                                                                 \
      return fn_apply(eerr, e, s);                                       \
    }                                                                    \
    int m = IS.chunkLength(actual);                                      \
    s = maybe.value.e2;                                                  \
    if (!m) {                                                            \
      return fn_apply(eok, actual, s, NULL);                             \
    }                                                                    \
    return fn_apply(cok, actual, s, NULL);                               \
  }                                                                      \
                                                                         \
  static Parsec(S, Tokens(S)) FUNC_NAME(tokens, S)(                      \
      Fn(Tokens(S), Tokens(S), bool) test, Tokens(S) pattern) {          \
    __auto_type f = FUNC_NAME(tokensImpl, S)();                          \
    return (Parsec(S, Tokens(S))){                                       \
        .unParser = fn_apply(f, test, pattern),                          \
    };                                                                   \
  }                                                                      \
                                                                         \
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
    return trait(ParsecToken(S, Token(S)))                               \
        .pSatisfyMap(fn_apply(f, t), FUNC_NAME(toHints, Token(S))(t));   \
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
    return trait(ParsecToken(S, Token(S)))                               \
        .pSatisfyMap(fn_apply(f, pred), (Hints(Token(S))){0});           \
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
/* anySingleBut(t) */
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

// -----------------------------------------------------------------------
/* chunk(pattern) */
#define impl_chunk(S)                                                    \
                                                                         \
  fn(FUNC_NAME(chunkTest, S), Tokens(S), Tokens(S), bool) {              \
    return trait(Eq(Tokens(S))).eq(args->e1, args->e2);                  \
  }                                                                      \
                                                                         \
  static Parsec(S, Tokens(S)) FUNC_NAME(chunk, S)(Tokens(S) chk) {       \
    __auto_type f = FUNC_NAME(chunkTest, S)();                           \
    return FUNC_NAME(tokens, S)(f, chk);                                 \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
