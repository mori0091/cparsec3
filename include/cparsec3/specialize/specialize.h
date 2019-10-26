/* -*- coding: utf-8-unix -*- */
#pragma once

#if !defined(CPARSEC_STREAM_TYPE)
#error "CPARSEC_STREAM_TYPE not defined"
#else

#include "../core/core.h"
#include "../parsec/token.h"

#define Parser(T) Parsec(CPARSEC_STREAM_TYPE, T)
#define parseTest_s(T) PARSE_TEST(CPARSEC_STREAM_TYPE, T)
#define runParser_s(T) RUN_PARSER(CPARSEC_STREAM_TYPE, T)
#define runParserP_s(T) RUN_PARSERP(CPARSEC_STREAM_TYPE, T)

C_API_BEGIN
// -------------------------------------------------------------
#define typedef_Parser(T)                                                \
  typedef_Parsec(CPARSEC_STREAM_TYPE, T);                                \
  declare_token(CPARSEC_STREAM_TYPE, T);                                 \
  END_OF_STATEMENTS
FOREACH(typedef_Parser, TYPESET(ALL));
#undef typedef_Parser
// -------------------------------------------------------------
C_API_END

#define parse runParser
#define parseTest(p, name, input)                                        \
  GENERIC((p), Parser, parseTest_s, TYPESET(ALL))((p), (name), (input))
#define runParser(p, name, input)                                        \
  GENERIC((p), Parser, runParser_s, TYPESET(ALL))((p), (name), (input))
#define runParserP(p, state)                                             \
  GENERIC((p), Parser, runParserP_s, TYPESET(ALL))((p), (state))

#define define_CPARSEC(T)                                                \
  define_token(CPARSEC_STREAM_TYPE, T);                                  \
  END_OF_STATEMENTS

#define CPARSEC_DEFINE_ALL()                                             \
  FOREACH(define_CPARSEC, TYPESET(ALL));                                 \
  END_OF_STATEMENTS

#endif
