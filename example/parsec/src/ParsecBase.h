/* -*- coding: utf-8-unix -*- */
#include <cparsec3/base/base.h>

#include "parsec.h"

// -----------------------------------------------------------------------
#define trait_ParsecBase(S)                                              \
  C_API_BEGIN                                                            \
                                                                         \
  typedef_PosState(S);                                                   \
  typedef_ParseState(S);                                                 \
  typedef_ParseError(S);                                                 \
                                                                         \
  typedef_Parsec(S, None);                                               \
  typedef_Parsec(S, Token(S));                                           \
  typedef_Parsec(S, Tokens(S));                                          \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecBase(S)                                               \
  C_API_BEGIN                                                            \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
