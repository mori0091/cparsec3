/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// ParsecToken(S, T)
#define TRAIT_PARSECTOKEN(S, T) trait(ParsecToken(S, T))
#define satisfyMap(tmap, expect)                                         \
  GENERIC(tmap, FnMapToken, TRAIT_PARSECTOKEN,                           \
          BIND(S, PARSER_RETURN_TYPES(S)))                               \
      .satisfyMap(tmap, expect)

// -----------------------------------------------------------------------
// ParsecToken1(S)
#define tokens(cmp, pat)                                                 \
  trait(ParsecPrim1(CPARSEC_STREAM_TYPE)).tokens(cmp, pat)
#define takeWhileP(name_or_none, pred)                                   \
  trait(ParsecPrim1(CPARSEC_STREAM_TYPE)).takeWhileP(name_or_none, pred)
#define takeWhile1P(name_or_none, pred)                                  \
  trait(ParsecPrim1(CPARSEC_STREAM_TYPE)).takeWhile1P(name_or_none, pred)
#define takeP(name_or_none, n)                                           \
  trait(ParsecPrim1(CPARSEC_STREAM_TYPE)).takeP(name_or_none, n)
#define eof() trait(ParsecPrim1(CPARSEC_STREAM_TYPE)).eof()

#define single(t) trait(ParsecToken1(CPARSEC_STREAM_TYPE)).single(t)
#define satisfy(pred)                                                    \
  trait(ParsecToken1(CPARSEC_STREAM_TYPE)).satisfy(pred)
#define anySingle() trait(ParsecToken1(CPARSEC_STREAM_TYPE)).anySingle()
#define anySingleBut(t)                                                  \
  trait(ParsecToken1(CPARSEC_STREAM_TYPE)).anySingleBut(t)
#define oneOf(ts) trait(ParsecToken1(CPARSEC_STREAM_TYPE)).oneOf(ts)
#define noneOf(ts) trait(ParsecToken1(CPARSEC_STREAM_TYPE)).noneOf(ts)
#define chunk(chk) trait(ParsecToken1(CPARSEC_STREAM_TYPE)).chunk(chk)
#define takeRest() trait(ParsecToken1(CPARSEC_STREAM_TYPE)).takeRest()
