/* -*- coding: utf-8-unix -*- */
#pragma once

#include "base.h"

#define CREATE_TRAIT(...) trait(TYPE_NAME(__VA_ARGS__))

// clang-format off
#define BIND_TYPESET(CONTAINER)                                          \
  BIND(CONTAINER, TYPESET(ALL),                                          \
       APPLY(Array, TYPESET(ALL)),                                       \
       APPLY(List, TYPESET(ALL)),                                        \
       APPLY(Maybe, TYPESET(ALL)))                                       \
// clang-format on

// clang-format off
#define GENERIC_TRAIT(x)                                                 \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT,                                    \
          BIND_TYPESET(Array),                                           \
          BIND_TYPESET(List),                                            \
          BIND_TYPESET(Maybe))
// clang-format on

#define GENERIC_TRAIT_ARRAY(x)                                           \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(Array))

#define GENERIC_TRAIT_LIST(x)                                            \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(List))

#define g_length(x) GENERIC_TRAIT(x).length(x)
#define g_null(x) GENERIC_TRAIT(x).null(x)
#define g_free(x) GENERIC_TRAIT(x).free(x)

#define g_begin(x) GENERIC_TRAIT_ARRAY(x).begin(x)
#define g_end(x) GENERIC_TRAIT_ARRAY(x).end(x)

#define g_cons(x, xs) GENERIC_TRAIT_LIST(xs).cons(x, xs)
#define g_head(xs) GENERIC_TRAIT_LIST(xs).head(xs)
#define g_tail(xs) GENERIC_TRAIT_LIST(xs).tail(xs)
#define g_drop(n, xs) GENERIC_TRAIT_LIST(xs).drop(n, xs)

#define g_array(T, ...)                                                  \
  trait(Array(T)).from_array(                                            \
      VARIADIC_SIZE(__VA_ARGS__),                                        \
      (T[VARIADIC_SIZE(__VA_ARGS__)]){__VA_ARGS__})

#define g_list(T, ...)                                                   \
  trait(List(T)).from_array(                                             \
      VARIADIC_SIZE(__VA_ARGS__),                                        \
      (T[VARIADIC_SIZE(__VA_ARGS__)]){__VA_ARGS__})
