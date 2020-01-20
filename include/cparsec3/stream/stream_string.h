/* -*- coding: utf-8-unix -*- */
/**
 * \file cparsec3/stream/stream_string.h
 * \brief Defines String as an instance of stream type-class S.
 *
 * | generic   | concrete       | description                 |
 * |-----------+----------------+-----------------------------|
 * | S         | String         | a stream type               |
 * | Token(S)  | Token(String)  | char                        |
 * | Tokens(S) | Tokens(String) | String                      |
 * |-----------+----------------+-----------------------------|
 * | Stream(S) | Stream(String) | a set of stream I/F         |
 *
 * Calling to `trait(Stream(String))` returns a Stream(String).
 *
 * A Stream(String) object is a struct contains the following functions:
 *   - null(String)
 *   - take1(String)
 *   - takeN(int, String)
 *
 */
#pragma once

#include "../parsec/parsec.h"

C_API_BEGIN
// -------------------------------------------------------------

/** Token(S) : A data element taken from stream of type S */
#define pToken_String char

/** Tokens(S) : A chunk of data elements taken from stream of type S */
#define pTokens_String String

/** S : A stream */
trait_Stream(String);

// -------------------------------------------------------------
C_API_END
