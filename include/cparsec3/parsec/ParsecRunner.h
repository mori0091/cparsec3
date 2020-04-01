/* -*- coding: utf-8-unix -*- */
#include "../base/base_generics.h"

#include "parsec.h"

// -----------------------------------------------------------------------
#define ParsecRunner(...) TYPE_NAME(ParsecRunner, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecRunner(S, T)                                         \
  C_API_BEGIN                                                            \
  typedef_Parsec(S, T);                                                  \
  trait_Show(ParseResult(S, T));                                         \
  /* ---- trait ParsecRunner(S, T) */                                    \
  typedef struct ParsecRunner(S, T) ParsecRunner(S, T);                  \
  struct ParsecRunner(S, T) {                                            \
    ParseReply(S, T) (*pRunParsec)(Parsec(S, T) p, S input);             \
    ParseResult(S, T) (*pRunParser)(Parsec(S, T) p, S input);            \
    bool (*pParseTest)(Parsec(S, T) p, S input);                         \
  };                                                                     \
  ParsecRunner(S, T) Trait(ParsecRunner(S, T));                          \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecRunner(S, T)                                          \
  C_API_BEGIN                                                            \
                                                                         \
  static void FUNC_NAME(toString, Show(ParseResult(S, T)))(              \
      CharBuff * b, ParseResult(S, T) x) {                               \
    if (!x.success) {                                                    \
      trait(Stream(S)).stringifyState(b, x.state);                       \
      trait(Show(ParseError(S))).toString(b, x.err);                     \
    } else {                                                             \
      trait(Show(T)).toString(b, x.ok);                                  \
    }                                                                    \
  }                                                                      \
  instance_Show(ParseResult(S, T),                                       \
                FUNC_NAME(toString, Show(ParseResult(S, T))));           \
                                                                         \
  impl_runParsec(S, T);                                                  \
  impl_runParser(S, T);                                                  \
  impl_parseTest(S, T);                                                  \
                                                                         \
  ParsecRunner(S, T) Trait(ParsecRunner(S, T)) {                         \
    return (ParsecRunner(S, T)){                                         \
        .pRunParsec = FUNC_NAME(runParsec, S, T),                        \
        .pRunParser = FUNC_NAME(runParser, S, T),                        \
        .pParseTest = FUNC_NAME(parseTest, S, T),                        \
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
        .consumed = consumed,                                            \
        .result.state = state,                                           \
        .result.success = true,                                          \
        .result.ok = a,                                                  \
    };                                                                   \
  }                                                                      \
                                                                         \
  typedef_Fn_r(bool, ContErr(S, T));                                     \
  fn(FUNC_NAME(replyErr, S, T), bool, ContErrArgs(S, T)) {               \
    g_bind((consumed, err, state), *args);                               \
    return (ParseReply(S, T)){                                           \
        .consumed = consumed,                                            \
        .result.state = state,                                           \
        .result.success = false,                                         \
        .result.err = err,                                               \
    };                                                                   \
  }                                                                      \
                                                                         \
  /* ---- runParsec(p, state) */                                         \
  static ParseReply(S, T)                                                \
      FUNC_NAME(runParsec, S, T)(Parsec(S, T) p, S input) {              \
    __auto_type cok = FUNC_NAME(replyOk, S, T)();                        \
    __auto_type cerr = FUNC_NAME(replyErr, S, T)();                      \
    __auto_type eok = FUNC_NAME(replyOk, S, T)();                        \
    __auto_type eerr = FUNC_NAME(replyErr, S, T)();                      \
    return fn_apply(                                                     \
        p.unParser, input,                                               \
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
  static ParseResult(S, T)                                               \
      FUNC_NAME(runParser, S, T)(Parsec(S, T) p, S input) {              \
    return FUNC_NAME(runParsec, S, T)(p, input).result;                  \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_parseTest(S, T)                                             \
  /* ---- parseTest(p, input) */                                         \
  static bool FUNC_NAME(parseTest, S, T)(Parsec(S, T) p, S input) {      \
    MemCtx mctx = mem_ctx_begin();                                       \
    ParseResult(S, T) result = FUNC_NAME(runParser, S, T)(p, input);     \
    printf("%s\n", trait(Show(ParseResult(S, T))).show(result));         \
    mem_ctx_end(mctx);                                                   \
    return result.success;                                               \
  }                                                                      \
  END_OF_STATEMENTS
