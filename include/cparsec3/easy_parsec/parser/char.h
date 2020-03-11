/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// ParsecChar(S)
#define newline() trait(ParsecChar(CPARSEC_STREAM_TYPE)).newline()
#define crlf() trait(ParsecChar(CPARSEC_STREAM_TYPE)).crlf()
#define eol() trait(ParsecChar(CPARSEC_STREAM_TYPE)).eol()
#define tab() trait(ParsecChar(CPARSEC_STREAM_TYPE)).tab()
#define space() trait(ParsecChar(CPARSEC_STREAM_TYPE)).space()
#define space1() trait(ParsecChar(CPARSEC_STREAM_TYPE)).space1()

#define control() trait(ParsecChar(CPARSEC_STREAM_TYPE)).control()
#define whitespace() trait(ParsecChar(CPARSEC_STREAM_TYPE)).whitespace()
#define upper() trait(ParsecChar(CPARSEC_STREAM_TYPE)).upper()
#define lower() trait(ParsecChar(CPARSEC_STREAM_TYPE)).lower()
#define letter() trait(ParsecChar(CPARSEC_STREAM_TYPE)).letter()
#define alphaNum() trait(ParsecChar(CPARSEC_STREAM_TYPE)).alphaNum()
#define printable() trait(ParsecChar(CPARSEC_STREAM_TYPE)).printable()
#define digit() trait(ParsecChar(CPARSEC_STREAM_TYPE)).digit()
#define binDigit() trait(ParsecChar(CPARSEC_STREAM_TYPE)).binDigit()
#define octDigit() trait(ParsecChar(CPARSEC_STREAM_TYPE)).octDigit()
#define hexDigit() trait(ParsecChar(CPARSEC_STREAM_TYPE)).hexDigit()

#define char1(c) trait(ParsecChar(CPARSEC_STREAM_TYPE)).char1(c)
#define string1(s) trait(ParsecChar(CPARSEC_STREAM_TYPE)).string1(s)
