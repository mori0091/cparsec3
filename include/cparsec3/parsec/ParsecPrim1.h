/* -*- coding: utf-8-unix -*- */
#include "../base/base_generics.h"

#include "parsec.h"

// -----------------------------------------------------------------------
#define ParsecPrim1(...) TYPE_NAME(ParsecPrim, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecPrim1(S)                                             \
  C_API_BEGIN                                                            \
                                                                         \
  /* tokensMatcher (for `tokens` parser) */                              \
  typedef_Fn_r(Tokens(S), Tokens(S), bool);                              \
  /* tokenPredicate (for `takeWhileP`, `takeWhile1P` parsers) */         \
  typedef_Fn_r(Token(S), bool);                                          \
                                                                         \
  typedef struct ParsecPrim1(S) ParsecPrim1(S);                          \
  struct ParsecPrim1(S) {                                                \
    Parsec(S, None) (*eof)(void);                                        \
    Parsec(S, Tokens(S)) (*tokens)(Fn(Tokens(S), Tokens(S), bool) test,  \
                                   Tokens(S) pattern);                   \
    Parsec(S, Tokens(S)) (*takeWhileP)(Maybe(String) name,               \
                                       Fn(Token(S), bool) pred);         \
    Parsec(S, Tokens(S)) (*takeWhile1P)(Maybe(String) name,              \
                                        Fn(Token(S), bool) pred);        \
    Parsec(S, Tokens(S)) (*takeP)(Maybe(String) name, int n);            \
  };                                                                     \
                                                                         \
  ParsecPrim1(S) Trait(ParsecPrim1(S));                                  \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecPrim1(S)                                              \
  C_API_BEGIN                                                            \
                                                                         \
  impl_tokens(S);                                                        \
                                                                         \
  ParsecPrim1(S) Trait(ParsecPrim1(S)) {                                 \
    return (ParsecPrim1(S)){                                             \
        .eof = 0,                                                        \
        .tokens = FUNC_NAME(tokens, S),                                  \
        .takeWhileP = 0,                                                 \
        .takeWhile1P = 0,                                                \
        .takeP = 0,                                                      \
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
