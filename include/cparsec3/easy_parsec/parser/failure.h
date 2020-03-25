/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../runner.h"

#include "../../parsec/parser/ParsecFailure.h"

// -----------------------------------------------------------------------
// ParsecFailure(S, T)
#define TRAIT_PARSECFAILURE(S, T) trait(ParsecFailure(S, T))

#define parseError(T, e)                                                 \
  TRAIT_PARSECFAILURE(CPARSEC_STREAM_TYPE, T).pParseError(e)
#define failure(T, unexpect, expect)                                     \
  TRAIT_PARSECFAILURE(CPARSEC_STREAM_TYPE, T).pFailure(unexpect, expect)
#define unexpected(T, item)                                              \
  TRAIT_PARSECFAILURE(CPARSEC_STREAM_TYPE, T).pUnexpected(item)

// -----------------------------------------------------------------------
BIND_FOR(trait_ParsecFailure, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));

#ifdef CPARSEC_CONFIG_IMPLEMENT
BIND_FOR(impl_ParsecFailure, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));
#endif
