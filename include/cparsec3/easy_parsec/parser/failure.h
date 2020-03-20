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
trait_ParsecFailure(CPARSEC_STREAM_TYPE, None);
trait_ParsecFailure(CPARSEC_STREAM_TYPE, Token(CPARSEC_STREAM_TYPE));
trait_ParsecFailure(CPARSEC_STREAM_TYPE, Tokens(CPARSEC_STREAM_TYPE));
trait_ParsecFailure(CPARSEC_STREAM_TYPE,
                    Array(Token(CPARSEC_STREAM_TYPE)));
trait_ParsecFailure(CPARSEC_STREAM_TYPE,
                    Array(Tokens(CPARSEC_STREAM_TYPE)));

#ifdef CPARSEC_CONFIG_IMPLEMENT
impl_ParsecFailure(CPARSEC_STREAM_TYPE, None);
impl_ParsecFailure(CPARSEC_STREAM_TYPE, Token(CPARSEC_STREAM_TYPE));
impl_ParsecFailure(CPARSEC_STREAM_TYPE, Tokens(CPARSEC_STREAM_TYPE));
impl_ParsecFailure(CPARSEC_STREAM_TYPE,
                   Array(Token(CPARSEC_STREAM_TYPE)));
impl_ParsecFailure(CPARSEC_STREAM_TYPE,
                   Array(Tokens(CPARSEC_STREAM_TYPE)));
#endif
