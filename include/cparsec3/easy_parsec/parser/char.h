/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../runner.h"
#include "choice.h"
#include "combinator.h"
#include "token.h"

#include "../../parsec/parser/ParsecChar.h"

// -----------------------------------------------------------------------
// ParsecChar(S)
#define newline() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pNewline()
#define crlf() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pCrlf()
#define eol() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pEol()
#define tab() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pTab()
#define space() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pSpace()
#define space1() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pSpace1()

#define control() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pControl()
#define whitespace() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pWhitespace()
#define upper() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pUpper()
#define lower() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pLower()
#define letter() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pLetter()
#define alphaNum() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pAlphaNum()
#define printable() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pPrintable()
#define digit() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pDigit()
#define binDigit() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pBinDigit()
#define octDigit() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pOctDigit()
#define hexDigit() trait(ParsecChar(CPARSEC_STREAM_TYPE)).pHexDigit()

#define char1(c) trait(ParsecChar(CPARSEC_STREAM_TYPE)).pChar1(c)
#define string1(s) trait(ParsecChar(CPARSEC_STREAM_TYPE)).pString1(s)

// -----------------------------------------------------------------------
trait_ParsecChar(CPARSEC_STREAM_TYPE);

#ifdef CPARSEC_CONFIG_IMPLEMENT
impl_ParsecChar(CPARSEC_STREAM_TYPE);
#endif
