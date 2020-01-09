/* -*- coding: utf-8-unix -*- */
#include "../base/base_generics.h"

#include "parsec.h"

// -----------------------------------------------------------------------
#define ParsecRunner(...) TYPE_NAME(ParsecRunner, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecRunner(S, T)                                         \
  C_API_BEGIN                                                            \
  /* ---- trait ParsecRunner(S, T) */                                    \
  typedef struct ParsecRunner(S, T) ParsecRunner(S, T);                  \
  struct ParsecRunner(S, T) {                                            \
    ParseReply(S, T) (*runParsec)(Parsec(S, T) p, ParseState(S) state);  \
    Result(T, ParseError(S)) (*runParser)(Parsec(S, T) p, String name,   \
                                          S input);                      \
    bool (*parseTest)(Parsec(S, T) p, S input);                          \
  };                                                                     \
  ParsecRunner(S, T) Trait(ParsecRunner(S, T));                          \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecRunner(S, T)                                          \
  C_API_BEGIN                                                            \
                                                                         \
  impl_runParsec(S, T);                                                  \
  impl_runParser(S, T);                                                  \
  impl_parseTest(S, T);                                                  \
                                                                         \
  ParsecRunner(S, T) Trait(ParsecRunner(S, T)) {                         \
    return (ParsecRunner(S, T)){                                         \
        .runParsec = FUNC_NAME(runParsec, S, T),                         \
        .runParser = FUNC_NAME(runParser, S, T),                         \
        .parseTest = FUNC_NAME(parseTest, S, T),                         \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_runParsec(S, T)                                             \
  typedef_Fn_r(bool, ContOk(S, T));                                      \
  fn(FUNC_NAME(replyOk, S, T), bool, ContOkArgs(S, T)) {                 \
    g_bind((consumed, a, state), *args);                                 \
    return (ParseReply(S, T)){                                           \
        .state = state,                                                  \
        .consumed = consumed,                                            \
        .result = {.success = true, .ok = a},                            \
    };                                                                   \
  }                                                                      \
                                                                         \
  typedef_Fn_r(bool, ContErr(S, T));                                     \
  fn(FUNC_NAME(replyErr, S, T), bool, ContErrArgs(S, T)) {               \
    g_bind((consumed, err, state), *args);                               \
    return (ParseReply(S, T)){                                           \
        .state = state,                                                  \
        .consumed = consumed,                                            \
        .result = {.err = err},                                          \
    };                                                                   \
  }                                                                      \
                                                                         \
  /* ---- runParsec(p, state) */                                         \
  static ParseReply(S, T)                                                \
      FUNC_NAME(runParsec, S, T)(Parsec(S, T) p, ParseState(S) state) {  \
    __auto_type cok = FUNC_NAME(replyOk, S, T)();                        \
    __auto_type cerr = FUNC_NAME(replyErr, S, T)();                      \
    __auto_type eok = FUNC_NAME(replyOk, S, T)();                        \
    __auto_type eerr = FUNC_NAME(replyErr, S, T)();                      \
    return fn_apply(                                                     \
        p.unParser, state,                                               \
        fn_apply(cok, true),  /* continuation 'consumed-ok' */           \
        fn_apply(cerr, true), /* continuation 'consumed-error' */        \
        fn_apply(eok, false), /* continuation 'empty-ok' */              \
        fn_apply(eerr, false) /* continuation 'empty-error' */           \
    );                                                                   \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_runParser(S, T)                                             \
  /* ---- runParser(p, name, input) */                                   \
  static Result(T, ParseError(S))                                        \
      FUNC_NAME(runParser, S, T)(Parsec(S, T) p, String name, S input) { \
    return FUNC_NAME(runParsec, S, T)(p, initialParseState(name, input)) \
        .result;                                                         \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_parseTest(S, T)                                             \
  /* ---- parseTest(p, input) */                                         \
  static bool FUNC_NAME(parseTest, S, T)(Parsec(S, T) p, S input) {      \
    __auto_type result = FUNC_NAME(runParser, S, T)(p, "", input);       \
    if (!result.success) {                                               \
      FUNC_NAME(print, ParseError(S))(result.err);                       \
      return false;                                                      \
    }                                                                    \
    printf("'%c'\n", result.ok); /* TODO implement Show(T) */            \
    return true;                                                         \
  }                                                                      \
  END_OF_STATEMENTS