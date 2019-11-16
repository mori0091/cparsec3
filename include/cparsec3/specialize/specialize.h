/* -*- coding: utf-8-unix -*- */
#pragma once

#if !defined(CPARSEC_STREAM_TYPE)
#error "CPARSEC_STREAM_TYPE not defined"
#else

#include "../core/core.h"
#include "../parsec/token.h"

// -------------------------------------------------------------
#define CPARSEC_PARSER_RETURN_TYPES(S) TYPESET(ALL), Token(S), Tokens(S)

// -------------------------------------------------------------
#define CPARSEC_DECLARE_ALL(S)                                           \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  FOREACH(trait_Eq, Token(S), Tokens(S));                                \
  FOREACH(trait_Ord, Token(S), Tokens(S));                               \
  FOREACH(trait_Mem, Token(S), Tokens(S));                               \
  FOREACH(trait_Array, Token(S), Tokens(S));                             \
  FOREACH(trait_List, Token(S), Tokens(S));                              \
  FOREACH(trait_Maybe, Token(S), Tokens(S));                             \
  /* ---- */                                                             \
  typedef_PosState(S);                                                   \
  typedef_ParseState(S);                                                 \
  trait_ParseErrorBundle(S);                                             \
  /* ---- */                                                             \
  FOREACH(typedef_Guard, Token(S), Tokens(S));                           \
  FOREACH(typedef_Trans,                                                 \
          BIND(Token(S), CPARSEC_PARSER_RETURN_TYPES(S)));               \
  FOREACH(typedef_Match,                                                 \
          BIND(Token(S), CPARSEC_PARSER_RETURN_TYPES(S)));               \
  /* ---- */                                                             \
  FOREACH(typedef_Parsec, BIND(S, CPARSEC_PARSER_RETURN_TYPES(S)));      \
  /* ---- */                                                             \
  FOREACH(declare_token, BIND(S, CPARSEC_PARSER_RETURN_TYPES(S)));       \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
// -------------------------------------------------------------
#define CPARSEC_DEFINE_ALL(S)                                            \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  FOREACH(impl_Mem, Token(S), Tokens(S));                                \
  FOREACH(impl_Array, Token(S), Tokens(S));                              \
  FOREACH(impl_List, Token(S), Tokens(S));                               \
  FOREACH(impl_Maybe, Token(S), Tokens(S));                              \
  /* ---- */                                                             \
  impl_ParseErrorBundle(S);                                              \
  /* ---- */                                                             \
  FOREACH(define_Guard, Token(S), Tokens(S));                            \
  FOREACH(define_Trans, BIND(Token(S), CPARSEC_PARSER_RETURN_TYPES(S))); \
  FOREACH(define_Match, BIND(Token(S), CPARSEC_PARSER_RETURN_TYPES(S))); \
  /* ---- */                                                             \
  /* ---- */                                                             \
  FOREACH(define_token, BIND(S, CPARSEC_PARSER_RETURN_TYPES(S)));        \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -------------------------------------------------------------
#define parse runParser

#define parseTest(p, name, input)                                        \
  GENERIC((p), Parsec, PARSE_TEST,                                       \
          BIND(CPARSEC_STREAM_TYPE,                                      \
               CPARSEC_PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE)))        \
  ((p), (name), (input))

#define runParser(p, name, input)                                        \
  GENERIC((p), Parsec, RUN_PARSER,                                       \
          BIND(CPARSEC_STREAM_TYPE,                                      \
               CPARSEC_PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE)))        \
  ((p), (name), (input))

#define runParserP(p, state)                                             \
  GENERIC((p), Parsec, RUN_PARSERP,                                      \
          BIND(CPARSEC_STREAM_TYPE,                                      \
               CPARSEC_PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE)))        \
  ((p), (state))

// -------------------------------------------------------------

#endif
