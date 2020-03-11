/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base_generics.h>

#include <cparsec3/parsec/stream.h>

#include <cparsec3/parsec/ParsecBase.h>
#include <cparsec3/parsec/ParsecRunner.h>

#include <cparsec3/parsec/ParsecFailure.h>
#include <cparsec3/parsec/ParsecFailure1.h>

#include <cparsec3/parsec/ParsecPrim.h>
#include <cparsec3/parsec/ParsecPrim1.h>

#include <cparsec3/parsec/ParsecDeriv.h>

#include <cparsec3/parsec/ParsecChoice.h>

#include <cparsec3/parsec/ParsecChar.h>

#include <cparsec3/parsec/ParsecRepeat.h>

// -----------------------------------------------------------------------
#define trait_ParsecLibrary(S)                                           \
  trait_ParsecBase(S);                                                   \
                                                                         \
  trait_ParsecRunner(S, None);                                           \
  trait_ParsecRunner(S, Token(S));                                       \
  trait_ParsecRunner(S, Tokens(S));                                      \
  trait_ParsecRunner(S, Array(Token(S)));                                \
  trait_ParsecRunner(S, Array(Tokens(S)));                               \
                                                                         \
  trait_ParsecPrim1(S);                                                  \
                                                                         \
  trait_ParsecPrim(S, None);                                             \
  trait_ParsecPrim(S, Token(S));                                         \
  trait_ParsecPrim(S, Tokens(S));                                        \
  trait_ParsecPrim(S, Array(Token(S)));                                  \
  trait_ParsecPrim(S, Array(Tokens(S)));                                 \
                                                                         \
  trait_ParsecFailure1(S);                                               \
  trait_ParsecFailure(S, Token(S));                                      \
  trait_ParsecFailure(S, Tokens(S));                                     \
                                                                         \
  trait_ParsecDeriv(S);                                                  \
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
  impl_ParsecBase(S);                                                    \
                                                                         \
  impl_ParsecRunner(S, None);                                            \
  impl_ParsecRunner(S, Token(S));                                        \
  impl_ParsecRunner(S, Tokens(S));                                       \
  impl_ParsecRunner(S, Array(Token(S)));                                 \
  impl_ParsecRunner(S, Array(Tokens(S)));                                \
                                                                         \
  impl_ParsecPrim1(S);                                                   \
                                                                         \
  impl_ParsecPrim(S, None);                                              \
  impl_ParsecPrim(S, Token(S));                                          \
  impl_ParsecPrim(S, Tokens(S));                                         \
  impl_ParsecPrim(S, Array(Token(S)));                                   \
  impl_ParsecPrim(S, Array(Tokens(S)));                                  \
                                                                         \
  impl_ParsecFailure1(S);                                                \
  impl_ParsecFailure(S, Token(S));                                       \
  impl_ParsecFailure(S, Tokens(S));                                      \
                                                                         \
  impl_ParsecDeriv(S);                                                   \
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
