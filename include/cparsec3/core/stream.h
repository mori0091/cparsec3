/* -*- coding: utf-8-unix -*- */
#pragma once

#include "core.h"

#define Stream(S) TYPE_NAME(Stream, S)
#define Token(S) TYPE_NAME(Token, S)
#define Tokens(S) TYPE_NAME(Tokens, S)

#define trait_Stream(S)                                                  \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_Tuple(Token(S), S);                                            \
  typedef_Tuple(Tokens(S), S);                                           \
  typedef_Maybe(Tuple(Token(S), S));                                     \
  typedef_Maybe(Tuple(Tokens(S), S));                                    \
  /* ---- */                                                             \
  typedef struct {                                                       \
    bool (*empty)(S s);                                                  \
    Maybe(Tuple(Token(S), S)) (*take1)(S s);                             \
    Maybe(Tuple(Tokens(S), S)) (*takeN)(int n, S s);                     \
  } Stream(S);                                                           \
  Stream(S) Trait(Stream(S));                                            \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
