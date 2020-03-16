/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// ParsecFailure(S, T)
#define TRAIT_PARSECFAILURE(S, T) trait(ParsecFailure(S, T))

#define parseError(T, e)                                                 \
  TRAIT_PARSECFAILURE(CPARSEC_STREAM_TYPE, T).pParseError(e)
#define failure(T, unexpect, expect)                                     \
  TRAIT_PARSECFAILURE(CPARSEC_STREAM_TYPE, T).pFailure(unexpect, expect)
#define unexpected(T, item)                                              \
  TRAIT_PARSECFAILURE(CPARSEC_STREAM_TYPE, T).pUnexpected(item)
