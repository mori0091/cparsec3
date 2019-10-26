/* -*- coding: utf-8-unix -*- */
/**
 * \file cparsec3/stream/stream_string.h
 * \brief Defines String as an instance of stream type-class S.
 *
 * | generic   | concrete       | description                 |
 * |-----------+----------------+-----------------------------|
 * | S         | String         | a stream type               |
 * | Token(S)  | Token(String)  | char                        |
 * | Tokens(S) | Tokens(String) | type of a chunk of `char`s. |
 * |-----------+----------------+-----------------------------|
 * | Stream(S) | Stream(String) | a set of stream I/F         |
 *
 * Calling to `cparsec_module(Stream(String))` returns a Stream(String).
 *
 * A Stream(String) object is a struct contains the following functions:
 *   - empty(String)
 *   - take1(String)
 *   - takeN(int, String)
 *
 */
#pragma once

#include "../core/core.h"

C_API_BEGIN
// -------------------------------------------------------------

/** Token(S) : A data element taken from stream of type S */
typedef char Token(String);

/** Tokens(S) : A chunk of data elements taken from stream of type S */
typedef struct {
  size_t length;
  const char* value;
} Tokens(String);

/** S : A stream */
declare_Stream(String);

// -------------------------------------------------------------
C_API_END
