/* -*- coding: utf-8-unix -*- */
#include "../base/base_generics.h"

#include "parsec.h"

// -----------------------------------------------------------------------
#define ParsecPrim(...) TYPE_NAME(ParsecPrim, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecPrim(S, T)                                           \
  C_API_BEGIN                                                            \
  /* testToken (for `token` parser) */                                   \
  typedef_Fn_r(Token(S), Maybe(T));                                      \
  /* tokensMatcher (for `tokens` parser) */                              \
  typedef_Fn_r(Tokens(String), Tokens(String), bool);                    \
  /* tokenPredicate (for `takeWhileP`, `takeWhile1P` parsers) */         \
  typedef_Fn_r(Token(String), bool);                                     \
                                                                         \
  typedef struct ParsecPrim(S, T) ParsecPrim(S, T);                      \
  struct ParsecPrim(S, T) {                                              \
    Parsec(S, T) (*parseError)(ParseError(S) e);                         \
    Parsec(S, T) (*label)(String name, Parsec(S, T) p);                  \
    Parsec(S, T) (*hidden)(Parsec(S, T) p);                              \
    Parsec(S, T) (*tryp)(Parsec(S, T) p);                                \
    Parsec(S, T) (*lookAhead)(Parsec(S, T) p);                           \
    Parsec(S, None) (*notFollowedBy)(Parsec(S, T) p);                    \
    Parsec(S, None) (*eof)(void);                                        \
    Parsec(S, T) (*token)(Fn(Token(S), Maybe(T)) testToken,              \
                          Hints(Token(S)) expecting);                    \
    Parsec(S, Tokens(S)) (*tokens)(Fn(Tokens(S), Tokens(S), bool),       \
                                   Tokens(S));                           \
    Parsec(S, Tokens(S)) (*takeWhileP)(Maybe(String) name,               \
                                       Fn(Token(S), bool) pred);         \
    Parsec(S, Tokens(S)) (*takeWhile1P)(Maybe(String) name,              \
                                        Fn(Token(S), bool) pred);        \
    Parsec(S, Tokens(S)) (*takeP)(Maybe(String) name, int n);            \
  };                                                                     \
                                                                         \
  ParsecPrim(S, T) Trait(ParsecPrim(S, T));                              \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecPrim(S, T)                                            \
  C_API_BEGIN                                                            \
                                                                         \
  impl_parseError(S, T);                                                 \
  impl_tryp(S, T);                                                       \
  impl_token(S, T);                                                      \
                                                                         \
  ParsecPrim(S, T) Trait(ParsecPrim(S, T)) {                             \
    return (ParsecPrim(S, T)){                                           \
        .parseError = FUNC_NAME(parseError, S, T),                       \
        .label = 0,                                                      \
        .hidden = 0,                                                     \
        .tryp = FUNC_NAME(tryp, S, T),                                   \
        .lookAhead = 0,                                                  \
        .notFollowedBy = 0,                                              \
        .eof = 0,                                                        \
        .token = FUNC_NAME(token, S, T),                                 \
        .tokens = 0,                                                     \
        .takeWhileP = 0,                                                 \
        .takeWhile1P = 0,                                                \
        .takeP = 0,                                                      \
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

// -----------------------------------------------------------------------
/* tryp(p) */
#define impl_tryp(S, T)                                                  \
  typedef_Fn_r(ContErr(S, T), ParseState(S), ContErr(S, T));             \
  fn(FUNC_NAME(contErrBackTrack, S, T), /* name */                       \
     ContErr(S, T),    /* f : underlying continuation */                 \
     ParseState(S),    /* s : state that overrides the given state */    \
     ContErrArgs(S, T) /* error -> state -> reply */                     \
  ) {                                                                    \
    g_bind((f, s, e), *args);                                            \
    return fn_apply(f, e, s);                                            \
  }                                                                      \
                                                                         \
  typedef_Fn_r(Parsec(S, T), UnParser(S, T));                            \
  fn(FUNC_NAME(trypImpl, S, T), /* name */                               \
     Parsec(S, T),              /* p : underlying parser */              \
     UnParserArgs(S,                                                     \
                  T) /* state -> cok -> cerr -> eok -> eerr -> reply */  \
  ) {                                                                    \
    g_bind((p, s, cok, , eok, eerr), *args);                             \
    __auto_type f = FUNC_NAME(contErrBackTrack, S, T)();                 \
    __auto_type eerr_ = fn_apply(f, eerr, s);                            \
    return fn_apply(p.unParser, s, cok, eerr_, eok, eerr_);              \
  }                                                                      \
                                                                         \
  static Parsec(S, T) FUNC_NAME(tryp, S, T)(Parsec(S, T) p) {            \
    __auto_type f = FUNC_NAME(trypImpl, S, T)();                         \
    return (Parsec(S, T)){.unParser = fn_apply(f, p)};                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* token(testToken, expecting) */
#define impl_token(S, T)                                                 \
  typedef_Fn_r(Fn(Token(S), Maybe(T)), Hints(Token(S)), UnParser(S, T)); \
  fn(FUNC_NAME(tokenImpl, S, T), Fn(Token(S), Maybe(T)),                 \
     Hints(Token(S)), UnParserArgs(S, T)) {                              \
    g_bind((testToken, expect, s, cok, cerr, , eerr), *args);            \
    __auto_type maybe = trait(Stream(S)).take1(s.input);                 \
    if (maybe.none) {                                                    \
      ParseError(S) e = {                                                \
          .offset = s.offset,                                            \
          .unexpected.value.type = END_OF_INPUT,                         \
          .expecting = expect,                                           \
      };                                                                 \
      return fn_apply(eerr, e, s);                                       \
    }                                                                    \
                                                                         \
    /* update state */                                                   \
    s.input = maybe.value.e2;                                            \
    s.offset++;                                                          \
                                                                         \
    Token(S) a = maybe.value.e1;                                         \
    __auto_type maybe2 = fn_apply(testToken, a);                         \
    if (maybe2.none) {                                                   \
      ParseError(S) e = {                                                \
          .offset = s.offset,                                            \
          .unexpected.value.type = TOKENS,                               \
          .unexpected.value.tokens =                                     \
              trait(List(Token(S))).cons(a, NULL),                       \
          .expecting = expect,                                           \
      };                                                                 \
      return fn_apply(cerr, e, s);                                       \
    }                                                                    \
    return fn_apply(cok, maybe2.value, s, NULL);                         \
  }                                                                      \
                                                                         \
  static Parsec(S, T) FUNC_NAME(token, S, T)(                            \
      Fn(Token(S), Maybe(T)) testToken, Hints(Token(S)) expecting) {     \
    __auto_type f = FUNC_NAME(tokenImpl, S, T)();                        \
    return (Parsec(S, T)){                                               \
        .unParser = fn_apply(f, testToken, expecting),                   \
    };                                                                   \
  }                                                                      \
  END_OF_STATEMENTS
