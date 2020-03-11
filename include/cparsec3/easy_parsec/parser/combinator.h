/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// ParsecPrim(S, T)
#define TRAIT_PARSECPRIM(S, T) trait(ParsecPrim(S, T))

#define GENERIC_PARSECPRIM(S, p)                                         \
  GENERIC(p, Parsec, TRAIT_PARSECPRIM, BIND(S, PARSER_RETURN_TYPES(S)))

#define label(l, p) GENERIC_PARSECPRIM(CPARSEC_STREAM_TYPE, p).label(l, p)
#define hidden(p) GENERIC_PARSECPRIM(CPARSEC_STREAM_TYPE, p).hidden(p)
#define tryp(p) GENERIC_PARSECPRIM(CPARSEC_STREAM_TYPE, p).tryp(p)
#define lookAhead(p)                                                     \
  GENERIC_PARSECPRIM(CPARSEC_STREAM_TYPE, p).lookAhead(p)
#define notFollowedBy(p)                                                 \
  GENERIC_PARSECPRIM(CPARSEC_STREAM_TYPE, p).notFollowedBy(p)
