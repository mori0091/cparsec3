/* -*- coding: utf-8-unix -*- */
#pragma once

#include "base.h"

#define CREATE_TRAIT(...) trait(TYPE_NAME(__VA_ARGS__))

#define BIND_TYPESET(CONTAINER) BIND(CONTAINER, TYPESET_0, TYPESET_1)

#define APPLY_TYPESET(CONTAINER) APPLY(CONTAINER, TYPESET_0, TYPESET_1)

// clang-format off
#define GENERIC_CONTAINER(x)                    \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT,           \
          BIND_TYPESET(Array),                  \
          BIND_TYPESET(List),                   \
          BIND_TYPESET(Maybe))
// clang-format on

// clang-format off
#define GENERIC_EQ(x)                           \
  GENERIC(x, SND, CREATE_TRAIT,                 \
          BIND(Eq, TYPESET(ALL_BUT_INT),        \
               APPLY_TYPESET(Array),            \
               APPLY_TYPESET(List),             \
               APPLY_TYPESET(Maybe)))
// clang-format on

// clang-format off
#define GENERIC_ORD(x)                          \
  GENERIC(x, SND, CREATE_TRAIT,                 \
          BIND(Ord, TYPESET(ALL_BUT_INT),       \
               APPLY_TYPESET(Array),            \
               APPLY_TYPESET(List),             \
               APPLY_TYPESET(Maybe)))
// clang-format on

#define GENERIC_ARRAY(x)                                                 \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(Array))

#define GENERIC_LIST(x)                                                  \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(List))

#define g_eq(a, b) GENERIC_EQ(a).eq(a, b)
#define g_neq(a, b) GENERIC_EQ(a).neq(a, b)

#define g_le(a, b) GENERIC_ORD(a).le(a, b)
#define g_lt(a, b) GENERIC_ORD(a).lt(a, b)
#define g_ge(a, b) GENERIC_ORD(a).ge(a, b)
#define g_gt(a, b) GENERIC_ORD(a).gt(a, b)
#define g_min(a, b) GENERIC_ORD(a).min(a, b)
#define g_max(a, b) GENERIC_ORD(a).max(a, b)
#define g_cmp(a, b) GENERIC_ORD(a).cmp(a, b)
#define g_compare(a, b) GENERIC_ORD(a).cmp(a, b)

#define g_length(x) GENERIC_CONTAINER(x).length(x)
#define g_null(x) GENERIC_CONTAINER(x).null(x)
#define g_free(x) GENERIC_CONTAINER(x).free(x)

#define g_begin(x) GENERIC_ARRAY(x).begin(x)
#define g_end(x) GENERIC_ARRAY(x).end(x)

#define g_cons(x, xs) GENERIC_LIST(xs).cons(x, xs)
#define g_head(xs) GENERIC_LIST(xs).head(xs)
#define g_tail(xs) GENERIC_LIST(xs).tail(xs)
#define g_drop(n, xs) GENERIC_LIST(xs).drop(n, xs)

#define g_array(T, ...)                                                  \
  trait(Array(T)).from_array(                                            \
      VARIADIC_SIZE(__VA_ARGS__),                                        \
      (T[VARIADIC_SIZE(__VA_ARGS__)]){__VA_ARGS__})

#define g_list(T, ...)                                                   \
  trait(List(T)).from_array(                                             \
      VARIADIC_SIZE(__VA_ARGS__),                                        \
      (T[VARIADIC_SIZE(__VA_ARGS__)]){__VA_ARGS__})
