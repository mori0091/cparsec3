/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../runner.h"

#include "../../parsec/parser/ParsecCombinator.h"

// -----------------------------------------------------------------------
// ParsecCombinator(S, T)
#define TRAIT_PARSECCOMBINATOR(S, T) trait(ParsecCombinator(S, T))

#define GENERIC_PARSECCOMBINATOR(S, p)                                   \
  GENERIC(p, Parsec, TRAIT_PARSECCOMBINATOR,                             \
          BIND(S, PARSER_RETURN_TYPES(S)))

#define label(l, p)                                                      \
  GENERIC_PARSECCOMBINATOR(CPARSEC_STREAM_TYPE, p).pLabel(l, p)
#define hidden(p)                                                        \
  GENERIC_PARSECCOMBINATOR(CPARSEC_STREAM_TYPE, p).pHidden(p)
#define tryp(p) GENERIC_PARSECCOMBINATOR(CPARSEC_STREAM_TYPE, p).pTryp(p)
#define lookAhead(p)                                                     \
  GENERIC_PARSECCOMBINATOR(CPARSEC_STREAM_TYPE, p).pLookAhead(p)
#define notFollowedBy(p)                                                 \
  GENERIC_PARSECCOMBINATOR(CPARSEC_STREAM_TYPE, p).pNotFollowedBy(p)

// -----------------------------------------------------------------------
BIND_FOR(trait_ParsecCombinator, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));

#ifdef CPARSEC_CONFIG_IMPLEMENT
BIND_FOR(impl_ParsecCombinator, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));
#endif
