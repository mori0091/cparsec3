/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// ParsecChoice(S, T)

#define TRAIT_PARSECCHOICE(S, T) trait(ParsecChoice(S, T))

#define GENERIC_PARSECCHOICE(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECCHOICE, BIND(S, PARSER_RETURN_TYPES(S)))

#define either(p1, p2)                                                   \
  GENERIC_PARSECCHOICE(CPARSEC_STREAM_TYPE, p1).either(p1, p2)
#define choice(...) FOLDL(either, __VA_ARGS__)
