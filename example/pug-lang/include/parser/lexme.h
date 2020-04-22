/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "blank.h"

C_API_BEGIN

#define GENERIC_LEXME(T) FUNC_NAME(lexme, T)
#define lexme(p) GENERIC(p, PARSER, GENERIC_LEXME, char, String)(p)

#define decl_lexme(T) PARSER(T) FUNC_NAME(lexme, T)(PARSER(T) p)

decl_lexme(char);
decl_lexme(String);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

#define impl_lexme(T)                                                    \
  parsec(FUNC_NAME(lexme, T), PARSER(T), T) {                            \
    DO() WITH(p) {                                                       \
      SCAN(p, x);                                                        \
      SCAN(blank());                                                     \
      RETURN(x);                                                         \
    }                                                                    \
  }                                                                      \
  END_OF_STATEMENTS

impl_lexme(char);
impl_lexme(String);

#endif

C_API_END
