/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../runner.h"
#include "failure.h"
#include "combinator.h"

#include "../../parsec/parser/ParsecChoice.h"

// -----------------------------------------------------------------------
// ParsecChoice(S, T)

#define TRAIT_PARSECCHOICE(S, T) trait(ParsecChoice(S, T))

#define GENERIC_PARSECCHOICE(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECCHOICE, BIND(S, PARSER_RETURN_TYPES(S)))

#define either(p1, p2)                                                   \
  GENERIC_PARSECCHOICE(CPARSEC_STREAM_TYPE, p1).pEither(p1, p2)
#define choice(...) FOLDL(either, __VA_ARGS__)

// -----------------------------------------------------------------------
trait_ParsecChoice(CPARSEC_STREAM_TYPE, None);
trait_ParsecChoice(CPARSEC_STREAM_TYPE, Token(CPARSEC_STREAM_TYPE));
trait_ParsecChoice(CPARSEC_STREAM_TYPE, Tokens(CPARSEC_STREAM_TYPE));
trait_ParsecChoice(CPARSEC_STREAM_TYPE,
                   Array(Token(CPARSEC_STREAM_TYPE)));
trait_ParsecChoice(CPARSEC_STREAM_TYPE,
                   Array(Tokens(CPARSEC_STREAM_TYPE)));

#ifdef CPARSEC_CONFIG_IMPLEMENT
impl_ParsecChoice(CPARSEC_STREAM_TYPE, None);
impl_ParsecChoice(CPARSEC_STREAM_TYPE, Token(CPARSEC_STREAM_TYPE));
impl_ParsecChoice(CPARSEC_STREAM_TYPE, Tokens(CPARSEC_STREAM_TYPE));
impl_ParsecChoice(CPARSEC_STREAM_TYPE, Array(Token(CPARSEC_STREAM_TYPE)));
impl_ParsecChoice(CPARSEC_STREAM_TYPE,
                  Array(Tokens(CPARSEC_STREAM_TYPE)));
#endif
