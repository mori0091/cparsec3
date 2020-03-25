/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../runner.h"
#include "combinator.h"
#include "failure.h"

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
BIND_FOR(trait_ParsecChoice, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));

#ifdef CPARSEC_CONFIG_IMPLEMENT
BIND_FOR(impl_ParsecChoice, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));
#endif
