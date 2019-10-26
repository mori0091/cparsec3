/* -*- coding: utf-8-unix -*- */
#pragma once

#include "core.h"

#define Stream(S) CONCAT(Stream, S)
#define Token(S) CONCAT(Token, S)
#define Tokens(S) CONCAT(Tokens, S)

#define declare_Stream(S)                                                \
  C_API_BEGIN                                                            \
  typedef_Tuple(Token(S), S);                                            \
  typedef_Tuple(Tokens(S), S);                                           \
  typedef_Maybe(Tuple(Token(S), S));                                     \
  typedef_Maybe(Tuple(Tokens(S), S));                                    \
  typedef struct {                                                       \
    bool (*empty)(S s);                                                  \
    Maybe(Tuple(Token(S), S)) (*take1)(S s);                             \
    Maybe(Tuple(Tokens(S), S)) (*takeN)(int n, S s);                     \
  } Stream(S);                                                           \
  Stream(S) CPARSEC_MODULE(Stream(S))(void);                             \
  /* ---- */                                                             \
  typedef_PosState(S);                                                   \
  typedef_ParseState(S);                                                 \
  typedef_ParseErrorBundle(S);                                           \
  C_API_END                                                              \
  END_OF_STATEMENTS
