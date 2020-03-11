/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// ParsecPrim1(S)
#define eof() trait(ParsecPrim1(CPARSEC_STREAM_TYPE)).eof()

// -----------------------------------------------------------------------
// ParsecDeriv(S)
#define single(t) trait(ParsecDeriv(CPARSEC_STREAM_TYPE)).single(t)
#define satisfy(pred)                                                    \
  trait(ParsecDeriv(CPARSEC_STREAM_TYPE)).satisfy(pred)
#define anySingle() trait(ParsecDeriv(CPARSEC_STREAM_TYPE)).anySingle()
#define anySingleBut(t)                                                  \
  trait(ParsecDeriv(CPARSEC_STREAM_TYPE)).anySingleBut(t)
#define oneOf(ts) trait(ParsecDeriv(CPARSEC_STREAM_TYPE)).oneOf(ts)
#define noneOf(ts) trait(ParsecDeriv(CPARSEC_STREAM_TYPE)).noneOf(ts)
#define chunk(chk) trait(ParsecDeriv(CPARSEC_STREAM_TYPE)).chunk(chk)
#define takeRest() trait(ParsecDeriv(CPARSEC_STREAM_TYPE)).takeRest()
