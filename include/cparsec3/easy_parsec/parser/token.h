/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../runner.h"

#include "../../parsec/parser/ParsecToken.h"
#include "../../parsec/parser/ParsecToken1.h"

// -----------------------------------------------------------------------
// ParsecToken(S, T)
#define TRAIT_PARSECTOKEN(S, T) trait(ParsecToken(S, T))
#define satisfyMap(tmap, expect)                                         \
  GENERIC(tmap, FnMapToken, TRAIT_PARSECTOKEN,                           \
          BIND(S, PARSER_RETURN_TYPES(S)))                               \
      .pSatisfyMap(tmap, expect)

// -----------------------------------------------------------------------
// ParsecToken1(S)
#define tokens(cmp, pat)                                                 \
  trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pTokens(cmp, pat)
#define takeWhileP(name_or_none, pred)                                   \
  trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pTakeWhileP(name_or_none, pred)
#define takeWhile1P(name_or_none, pred)                                  \
  trait(ParsecToken1(CPARSEC_STREAM_TYPE))                               \
      .pTakeWhile1P(name_or_none, pred)
#define takeP(name_or_none, n)                                           \
  trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pTakeP(name_or_none, n)
#define eof() trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pEof()

#define single(t) trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pSingle(t)
#define satisfy(pred)                                                    \
  trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pSatisfy(pred)
#define anySingle() trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pAnySingle()
#define anySingleBut(t)                                                  \
  trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pAnySingleBut(t)
#define oneOf(ts) trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pOneOf(ts)
#define noneOf(ts) trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pNoneOf(ts)
#define chunk(chk) trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pChunk(chk)
#define takeRest() trait(ParsecToken1(CPARSEC_STREAM_TYPE)).pTakeRest()

// -----------------------------------------------------------------------
trait_ParsecToken1(CPARSEC_STREAM_TYPE);
trait_ParsecToken(CPARSEC_STREAM_TYPE, None);
trait_ParsecToken(CPARSEC_STREAM_TYPE, Token(CPARSEC_STREAM_TYPE));
trait_ParsecToken(CPARSEC_STREAM_TYPE, Tokens(CPARSEC_STREAM_TYPE));
trait_ParsecToken(CPARSEC_STREAM_TYPE, Array(Token(CPARSEC_STREAM_TYPE)));
trait_ParsecToken(CPARSEC_STREAM_TYPE,
                  Array(Tokens(CPARSEC_STREAM_TYPE)));

#ifdef CPARSEC_CONFIG_IMPLEMENT
impl_ParsecToken1(CPARSEC_STREAM_TYPE);
impl_ParsecToken(CPARSEC_STREAM_TYPE, None);
impl_ParsecToken(CPARSEC_STREAM_TYPE, Token(CPARSEC_STREAM_TYPE));
impl_ParsecToken(CPARSEC_STREAM_TYPE, Tokens(CPARSEC_STREAM_TYPE));
impl_ParsecToken(CPARSEC_STREAM_TYPE, Array(Token(CPARSEC_STREAM_TYPE)));
impl_ParsecToken(CPARSEC_STREAM_TYPE, Array(Tokens(CPARSEC_STREAM_TYPE)));
#endif
