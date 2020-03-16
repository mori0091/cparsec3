/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// ParsecRepeat(S, T)

#define TRAIT_PARSECREPEAT(S, T) trait(ParsecRepeat(S, T))

#define GENERIC_PARSECREPEAT(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECREPEAT,                                 \
          BIND(S, PARSER_RETURN_TYPES_0(S)))

#define many(p) GENERIC_PARSECREPEAT(CPARSEC_STREAM_TYPE, p).pMany(p)
#define some(p) GENERIC_PARSECREPEAT(CPARSEC_STREAM_TYPE, p).pSome(p)
#define count(n, p)                                                      \
  GENERIC_PARSECREPEAT(CPARSEC_STREAM_TYPE, p).pCount(n, p)
#define count_min_max(m, n, p)                                           \
  GENERIC_PARSECREPEAT(CPARSEC_STREAM_TYPE, p).pCount_min_max(m, n, p)
