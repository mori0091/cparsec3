/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#define Stream(S) TYPE_NAME(Stream, S)
#define Token(S) TYPE_NAME(Token, S)
#define Tokens(S) TYPE_NAME(Tokens, S)

#define trait_Stream(S)                                                  \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_Tuple(Token(S), S);                                            \
  typedef_Tuple(Tokens(S), S);                                           \
  trait_Maybe(Tuple(Token(S), S));                                       \
  trait_Maybe(Tuple(Tokens(S), S));                                      \
  /* ---- */                                                             \
  typedef struct Stream(S) Stream(S);                                    \
  struct Stream(S) {                                                     \
    bool (*empty)(S s);                                                  \
    Maybe(Tuple(Token(S), S)) (*take1)(S s);                             \
    Maybe(Tuple(Tokens(S), S)) (*takeN)(int n, S s);                     \
  };                                                                     \
  Stream(S) Trait(Stream(S));                                            \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS