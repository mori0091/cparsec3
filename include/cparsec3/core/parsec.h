/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#include "parseerror.h"
#include "parsestate.h"
#include "posstate.h"
#include "sourcepos.h"
#include "stream.h"

// parseTest(p, name, input)
#define PARSE_TEST(S, T) CONCAT(cparsec_parseTest, S, T)
// runParser(p, name, input)
#define RUN_PARSER(S, T) CONCAT(cparsec_runParser, S, T)
// runParserP(p, state)
#define RUN_PARSERP(S, T) CONCAT(cparsec_runParserP, S, T)

#define PARSE_OK(S, T) CONCAT(cparsec_parseOk, S, T)
#define PARSE_ERR(S, T) CONCAT(cparsec_parseErr, S, T)
#define UNEXPECTED_EOM(S, T) CONCAT(cparsec_unexpectedEOM, S, T)
#define UNEXPECTED_TOKEN(S, T) CONCAT(cparsec_unexpectedToken, S, T)

#define ParseResult(S, T) CONCAT(ParseResult, S, T)
#define ParseStateResult(S, T) CONCAT(ParseStateResult, S, T)
#define PState(S) ParseState(S)
#define PResult(S, T) ParseResult(S, T)
#define PSResult(S, T) ParseStateResult(S, T)
#define PEBundle(S) ParseErrorBundle(S)

#define Parsec(S, T) CONCAT(Parsec, S, T)
#define typedef_Parsec(S, T)                                             \
  typedef_Result(T, PEBundle(S));                                        \
  typedef Result(T, PEBundle(S)) PResult(S, T);                          \
  typedef_Tupple(PState(S), Result(T, PEBundle(S)));                     \
  typedef Tupple(PState(S), Result(T, PEBundle(S))) PSResult(S, T);      \
  typedef struct {                                                       \
    PSResult(S, T) (*run)(void* arg, PState(S) state);                   \
    void* arg;                                                           \
  } Parsec(S, T);                                                        \
  /* runParserP(p, state) */                                             \
  static inline PSResult(S, T)                                           \
      RUN_PARSERP(S, T)(Parsec(S, T) p, PState(S) state) {               \
    return p.run(p.arg, state);                                          \
  }                                                                      \
  /* runParser(p, name, input) */                                        \
  static inline PResult(S, T)                                            \
      RUN_PARSER(S, T)(Parsec(S, T) p, String name, S input) {           \
    return (RUN_PARSERP(S, T)(p, initialParseState(name, input)))        \
        .second;                                                         \
  }                                                                      \
  /* parseTest(p, name, input) */                                        \
  static inline bool PARSE_TEST(S, T)(Parsec(S, T) p, String name,       \
                                      S input) {                         \
    /* TODO implement */                                                 \
    PResult(S, T) r = RUN_PARSER(S, T)(p, name, input);                  \
    if (r.success) {                                                     \
      /* print r.ok */                                                   \
      return true;                                                       \
    } else {                                                             \
      /* show error */                                                   \
      return false;                                                      \
    }                                                                    \
  }                                                                      \
  /* PARSE_OK(S,T)(state, ok) */                                         \
  static inline PSResult(S, T) PARSE_OK(S, T)(PState(S) state, T ok) {   \
    return (PSResult(S, T)){.first = (state),                            \
                            .second = {.success = true, .ok = (ok)}};    \
  }                                                                      \
  /* PARSE_ERR(S,T)(state, err) */                                       \
  static inline PSResult(S, T)                                           \
      PARSE_ERR(S, T)(PState(S) state, ParseError(S) err) {              \
    return (PSResult(S, T)){                                             \
        .first = (state),                                                \
        .second.err = (ParseErrorBundle(S)){                             \
            .errors = LList_CONS(ParseError(S))(err, NULL),              \
            .posState = (state).posState}};                              \
  }                                                                      \
  /* UNEXPECTED_EOM(S,T)(state, expecting) */                            \
  static inline PSResult(S, T) UNEXPECTED_EOM(S, T)(                     \
      PState(S) state, LList(ErrorItem(S)) expecting) {                  \
    return PARSE_ERR(S, T)(                                              \
        state, (ParseError(S)){.offset = state.offset,                   \
                               .unexpected.value.type = END_OF_INPUT,    \
                               .expecting = expecting});                 \
  }                                                                      \
  /* UNEXPECTED_TOKEN(S,T)(state, actual, expecting) */                  \
  static inline PSResult(S, T) UNEXPECTED_TOKEN(S, T)(                   \
      PState(S) state, Token(S) actual, LList(ErrorItem(S)) expecting) { \
    return PARSE_ERR(S, T)(                                              \
        state, (ParseError(S)){.offset = state.offset,                   \
                               .unexpected.value.type = TOKENS,          \
                               .unexpected.value.tokens =                \
                                   LList_CONS(Token(S))(actual, NULL),   \
                               .expecting = expecting});                 \
  }                                                                      \
  END_OF_STATEMENTS
