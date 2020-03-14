/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base_generics.h"

#include "../parsec/parsec.h"
#include "../parsec/stream.h"

#include "../parsec/ParsecRunner.h"

#include "../parsec/parser/ParsecFailure.h"

#include "../parsec/parser/ParsecToken.h"
#include "../parsec/parser/ParsecToken1.h"

#include "../parsec/parser/ParsecCombinator.h"

#include "../parsec/parser/ParsecChoice.h"

#include "../parsec/parser/ParsecChar.h"

#include "../parsec/parser/ParsecRepeat.h"

// -----------------------------------------------------------------------
#define trait_ParsecLibrary(S)                                           \
  trait_Maybe(Array(Token(S)));                                          \
  trait_Maybe(Array(Tokens(S)));                                         \
                                                                         \
  trait_ParseError(S);                                                   \
                                                                         \
  trait_ParsecRunner(S, None);                                           \
  trait_ParsecRunner(S, Token(S));                                       \
  trait_ParsecRunner(S, Tokens(S));                                      \
  trait_ParsecRunner(S, Array(Token(S)));                                \
  trait_ParsecRunner(S, Array(Tokens(S)));                               \
                                                                         \
  trait_ParsecToken1(S);                                                 \
  trait_ParsecToken(S, None);                                            \
  trait_ParsecToken(S, Token(S));                                        \
  trait_ParsecToken(S, Tokens(S));                                       \
  trait_ParsecToken(S, Array(Token(S)));                                 \
  trait_ParsecToken(S, Array(Tokens(S)));                                \
                                                                         \
  trait_ParsecCombinator(S, None);                                       \
  trait_ParsecCombinator(S, Token(S));                                   \
  trait_ParsecCombinator(S, Tokens(S));                                  \
  trait_ParsecCombinator(S, Array(Token(S)));                            \
  trait_ParsecCombinator(S, Array(Tokens(S)));                           \
                                                                         \
  trait_ParsecFailure(S, None);                                          \
  trait_ParsecFailure(S, Token(S));                                      \
  trait_ParsecFailure(S, Tokens(S));                                     \
  trait_ParsecFailure(S, Array(Token(S)));                               \
  trait_ParsecFailure(S, Array(Tokens(S)));                              \
                                                                         \
  trait_ParsecChoice(S, None);                                           \
  trait_ParsecChoice(S, Token(S));                                       \
  trait_ParsecChoice(S, Tokens(S));                                      \
  trait_ParsecChoice(S, Array(Token(S)));                                \
  trait_ParsecChoice(S, Array(Tokens(S)));                               \
                                                                         \
  trait_ParsecRepeat(S, Token(S));                                       \
  trait_ParsecRepeat(S, Tokens(S));                                      \
                                                                         \
  trait_ParsecChar(S);                                                   \
                                                                         \
  END_OF_STATEMENTS

#define impl_ParsecLibrary(S)                                            \
  /* impl_Maybe(Array(Token(S))); */                                     \
  /* impl_Maybe(Array(Tokens(S))); */                                    \
                                                                         \
  impl_ParseError(S);                                                    \
                                                                         \
  impl_ParsecRunner(S, None);                                            \
  impl_ParsecRunner(S, Token(S));                                        \
  impl_ParsecRunner(S, Tokens(S));                                       \
  impl_ParsecRunner(S, Array(Token(S)));                                 \
  impl_ParsecRunner(S, Array(Tokens(S)));                                \
                                                                         \
  impl_ParsecToken1(S);                                                  \
  impl_ParsecToken(S, None);                                             \
  impl_ParsecToken(S, Token(S));                                         \
  impl_ParsecToken(S, Tokens(S));                                        \
  impl_ParsecToken(S, Array(Token(S)));                                  \
  impl_ParsecToken(S, Array(Tokens(S)));                                 \
                                                                         \
  impl_ParsecCombinator(S, None);                                        \
  impl_ParsecCombinator(S, Token(S));                                    \
  impl_ParsecCombinator(S, Tokens(S));                                   \
  impl_ParsecCombinator(S, Array(Token(S)));                             \
  impl_ParsecCombinator(S, Array(Tokens(S)));                            \
                                                                         \
  impl_ParsecFailure(S, None);                                           \
  impl_ParsecFailure(S, Token(S));                                       \
  impl_ParsecFailure(S, Tokens(S));                                      \
  impl_ParsecFailure(S, Array(Token(S)));                                \
  impl_ParsecFailure(S, Array(Tokens(S)));                               \
                                                                         \
  impl_ParsecChoice(S, None);                                            \
  impl_ParsecChoice(S, Token(S));                                        \
  impl_ParsecChoice(S, Tokens(S));                                       \
  impl_ParsecChoice(S, Array(Token(S)));                                 \
  impl_ParsecChoice(S, Array(Tokens(S)));                                \
                                                                         \
  impl_ParsecRepeat(S, Token(S));                                        \
  impl_ParsecRepeat(S, Tokens(S));                                       \
                                                                         \
  impl_ParsecChar(S);                                                    \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#if 0
#define CPARSEC_STREAM_TYPE String
#define CPARSEC_STREAM_NEW(input) input
#else
#define CPARSEC_STREAM_TYPE State(String)
#define CPARSEC_STREAM_NEW(input) trait(State(String)).create(input)
#endif

#if !defined(CPARSEC_STREAM_TYPE)
#define CPARSEC_STREAM_TYPE String
#define CPARSEC_STREAM_NEW(input) input
#endif

// -----------------------------------------------------------------------
trait_ParsecLibrary(CPARSEC_STREAM_TYPE);
impl_ParsecLibrary(CPARSEC_STREAM_TYPE);
