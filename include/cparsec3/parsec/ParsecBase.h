/* -*- coding: utf-8-unix -*- */
#include "../base/base.h"

#include "parsec.h"

// -----------------------------------------------------------------------
#define trait_ParsecBase(S)                                              \
  C_API_BEGIN                                                            \
                                                                         \
  typedef_PosState(S);                                                   \
  typedef_ParseState(S);                                                 \
  trait_ParseError(S);                                                   \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecBase(S)                                               \
  C_API_BEGIN                                                            \
                                                                         \
  impl_ParseError(S);                                                    \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
