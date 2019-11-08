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
#include "stream/stream_string.h"

#define CPARSEC_STREAM_TYPE String
#include "specialize/specialize.h"

// -------------------------------------------------------------
CPARSEC_DECLARE_ALL(String);
