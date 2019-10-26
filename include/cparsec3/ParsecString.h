/* -*- coding: utf-8-unix -*- */
/**
 * \file ParsecString.h
 * \brief The Parsec library for parsing a string as an input stream.
 *
 * - stream type : String (i.e. `const char*`)
 * - token type  : Token(String) is `char`
 * - chunk type  : Tokens(String) is chunk of `char`s (not a string)
 */
#pragma once

#include "core/core.h"
#include "parsec/token.h"

C_API_BEGIN
// -------------------------------------------------------------

/** Token(S) : A data element taken from stream of type S */
typedef char Token(String);
/** Tokens(S) : A chunk of data elements taken from stream of type S */
typedef struct {
  size_t length;
  const char* value;
} Tokens(String);

declare_Stream(String);

// -------------------------------------------------------------
C_API_END

#define CPARSEC_STREAM_TYPE String
#include "specialize/specialize.h"
