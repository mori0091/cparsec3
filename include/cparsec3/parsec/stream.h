/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"
#include "posstate.h"

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
  trait_PosState(S);                                                     \
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
    /** update PosState */                                               \
    PosState(S) (*advanceTo)(Offset o, PosState(S) pst);                 \
    /** constructs text of the line */                                   \
    String (*lineTextOf)(PosState(S) pst);                               \
  };                                                                     \
                                                                         \
  Stream(S) Trait(Stream(S));                                            \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
