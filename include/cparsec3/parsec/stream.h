/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"
#include "IndexState.h"
#include "TextState.h"

#define Stream(S) TYPE_NAME(Stream, S)
#define Token(S) TYPE_NAME(Token, S)
#define Tokens(S) TYPE_NAME(Tokens, S)

#define trait_Stream(S)                                                  \
  C_API_BEGIN                                                            \
                                                                         \
  typedef_Tuple(Token(S), S);                                            \
  typedef_Tuple(Tokens(S), S);                                           \
  trait_Maybe(Tuple(Token(S), S));                                       \
  trait_Maybe(Tuple(Tokens(S), S));                                      \
                                                                         \
  typedef struct Stream(S) Stream(S);                                    \
  struct Stream(S) {                                                     \
    bool (*null)(S s);                                                   \
    List(Token(S)) (*chunkToTokens)(Tokens(S) chk);                      \
    int (*chunkLength)(Tokens(S) chk);                                   \
    String (*showTokens)(List(Token(S)) tokens);                         \
    Maybe(Tuple(Token(S), S)) (*take1)(S s);                             \
    Maybe(Tuple(Tokens(S), S)) (*takeN)(int n, S s);                     \
                                                                         \
    Offset (*offsetOf)(S s);                                             \
    void (*stringifyState)(CharBuff * b, S s);                           \
  };                                                                     \
                                                                         \
  Stream(S) Trait(Stream(S));                                            \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

C_API_BEGIN
// -------------------------------------------------------------

/**
 * Token(String) : Type of token taken from stream of String type.
 * \note `Token(String)` is alias of `char`.
 */
#define pToken_String char
/**
 * Tokens(String) : Type of chunk tokens taken from stream of String type.
 * \note `Tokens(String)` is alias of `String`.
 */
#define pTokens_String String
/**
 * Stream(String) trait.
 */
trait_Stream(String);

// -------------------------------------------------------------

#define pToken_pTextState_String char
#define pTokens_pTextState_String String
trait_Stream(TextState(String));

// -------------------------------------------------------------

#define pToken_pIndexState_String char
#define pTokens_pIndexState_String String
trait_Stream(IndexState(String));

// -------------------------------------------------------------
C_API_END
