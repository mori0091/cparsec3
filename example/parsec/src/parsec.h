/* -*- coding: utf-8-unix -*- */
#include <cparsec3/base/base.h>

#include <cparsec3/core/parseerror.h>
#include <cparsec3/core/parsestate.h>
#include <cparsec3/core/posstate.h>
#include <cparsec3/core/sourcepos.h>
#include <cparsec3/core/stream.h>

// -----------------------------------------------------------------------
#define ParseReply(...) TYPE_NAME(ParseReply, __VA_ARGS__)
#define Parsec(...) TYPE_NAME(Parsec, __VA_ARGS__)

// -----------------------------------------------------------------------
#define typedef_Parsec(S, T)                                             \
  typedef_Result(T, ParseError(S));                                      \
                                                                         \
  typedef struct ParseReply(S, T) ParseReply(S, T);                      \
  struct ParseReply(S, T) {                                              \
    ParseState(S) state;                                                 \
    bool consumed;                                                       \
    Result(T, ParseError(S)) result;                                     \
  };                                                                     \
                                                                         \
  typedef_ContOk(S, T);                                                  \
  typedef_ContErr(S, T);                                                 \
  typedef_UnParser(S, T);                                                \
  typedef struct Parsec(S, T) Parsec(S, T);                              \
  struct Parsec(S, T) {                                                  \
    UnParser(S, T) unParser;                                             \
  };                                                                     \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define ContOk(...) Fn(ContOkArgs(__VA_ARGS__))
#define typedef_ContOk(...) typedef_Fn_r(ContOkArgs(__VA_ARGS__))
#define ContOkArgs(S, T)                                                 \
  T,                   /* return type of parser */                       \
      ParseState(S),   /* next input state */                            \
      Hints(Token(S)), /* 'expecting' part of an error message */        \
      ParseReply(S, T) /* return type of the continuation */

// -----------------------------------------------------------------------
#define ContErr(...) Fn(ContErrArgs(__VA_ARGS__))
#define typedef_ContErr(...) typedef_Fn_r(ContErrArgs(__VA_ARGS__))
#define ContErrArgs(S, T)                                                \
  ParseError(S),       /* an error occurred */                           \
      ParseState(S),   /* next input state */                            \
      ParseReply(S, T) /* return type of the continuation */

// -----------------------------------------------------------------------
#define UnParser(...) Fn(UnParserArgs(__VA_ARGS__))
#define typedef_UnParser(...) typedef_Fn_r(UnParserArgs(__VA_ARGS__))
#define UnParserArgs(S, T)                                               \
  ParseState(S),       /* input state */                                 \
      ContOk(S, T),    /* continuation called when 'consumed-ok' */      \
      ContErr(S, T),   /* continuation called when 'consumed-error' */   \
      ContOk(S, T),    /* continuation called when 'empty-ok' */         \
      ContErr(S, T),   /* continuation called when 'empty-error' */      \
      ParseReply(S, T) /* return type of the continuation */
