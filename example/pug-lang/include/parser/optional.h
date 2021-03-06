/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

C_API_BEGIN

#define OPTIONAL_PARSER_RETURN_TYPES() None, char, String, Expr, Pat, Alt
#define GENERIC_OPTIONAL(T) FUNC_NAME(optional, T)
#define optional(p)                                                      \
  GENERIC(p, PARSER, GENERIC_OPTIONAL, OPTIONAL_PARSER_RETURN_TYPES())   \
  (p)

#define decl_optional(T)                                                 \
  PARSER(Maybe(T)) FUNC_NAME(optional, T)(PARSER(T) p)

decl_optional(None);
decl_optional(char);
decl_optional(String);
decl_optional(Expr);
decl_optional(Pat);
decl_optional(Alt);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

#define impl_optional(T)                                                 \
  parsec(FUNC_NAME(optional, T), PARSER(T), Maybe(T)) {                  \
    DO() WITH(p) {                                                       \
      SCAN(count_min_max(0, 1, p), m);                                   \
      if (!m.length) {                                                   \
        RETURN((Maybe(T)){.none = true});                                \
      }                                                                  \
      T x = m.data[0];                                                   \
      trait(Array(T)).free(&m);                                          \
      RETURN((Maybe(T)){.value = x});                                    \
    }                                                                    \
  }                                                                      \
  END_OF_STATEMENTS

impl_optional(None);
impl_optional(char);
impl_optional(String);
impl_optional(Expr);
impl_optional(Pat);
impl_optional(Alt);

#endif

C_API_END
