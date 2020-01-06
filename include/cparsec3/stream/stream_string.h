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
 * Calling to `trait(Stream(String))` returns a Stream(String).
 *
 * A Stream(String) object is a struct contains the following functions:
 *   - empty(String)
 *   - take1(String)
 *   - takeN(int, String)
 *
 */
#pragma once

#include "../parsec/parsec.h"

C_API_BEGIN
// -------------------------------------------------------------

/** Token(S) : A data element taken from stream of type S */
typedef char Token(String);

/** Tokens(S) : A chunk of data elements taken from stream of type S */
typedef struct Tokens(String) Tokens(String);
struct Tokens(String) {
  size_t length;
  const char* value;
};

/** S : A stream */
trait_Stream(String);

trait_Eq(Token(String));
trait_Ord(Token(String));
trait_Maybe(Token(String));
typedef Token(String) Item(List(Token(String)));
trait_List(Token(String));
trait_Array(Token(String));

// -------------------------------------------------------------
C_API_END
