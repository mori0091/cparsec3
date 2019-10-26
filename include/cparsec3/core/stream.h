/* -*- coding: utf-8-unix -*- */
#pragma once

#include "core.h"

#define Stream(S) CONCAT(Stream, S)
#define Token(S) CONCAT(Token, S)
#define Tokens(S) CONCAT(Tokens, S)

#define declare_Stream(S)                                                \
  C_API_BEGIN                                                            \
  typedef_Tupple(Token(S), S);                                           \
  typedef_Tupple(Tokens(S), S);                                          \
  typedef_Maybe(Tupple(Token(S), S));                                    \
  typedef_Maybe(Tupple(Tokens(S), S));                                   \
  typedef struct {                                                       \
    bool (*empty)(S s);                                                  \
    Maybe(Tupple(Token(S), S)) (*take1)(S s);                            \
    Maybe(Tupple(Tokens(S), S)) (*takeN)(int n, S s);                    \
  } Stream(S);                                                           \
  Stream(S) CPARSEC_MODULE(Stream(S))(void);                             \
  /* ---- */                                                             \
  typedef_PosState(S);                                                   \
  typedef_ParseState(S);                                                 \
  typedef_ParseErrorBundle(S);                                           \
  C_API_END                                                              \
  END_OF_STATEMENTS
