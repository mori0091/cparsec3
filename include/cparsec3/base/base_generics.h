/* -*- coding: utf-8-unix -*- */
#pragma once

#include "base.h"

#define TYPESET_CONTAINER TYPESET_1
#define TYPESET_ARRAY TYPESET_ARRAY_1
#define TYPESET_LIST TYPESET_LIST_1
#define TYPESET_SLICEABLE TYPESET_ARRAY, TYPESET_LIST
#define TYPESET_ITERABLE                                                 \
  TYPESET_SLICEABLE, APPLY(Slice, TYPESET_SLICEABLE)

#define IDENTITY(...) __VA_ARGS__

// clang-format off ------------------------------------------------------
#define TRAIT_EQ(T) trait(Eq(T))
#define GENERIC_EQ(x)                                                    \
  GENERIC(x, IDENTITY, TRAIT_EQ, TYPESET(PRIMITIVE), TYPESET_CONTAINER)

#define TRAIT_ORD(T) trait(Ord(T))
#define GENERIC_ORD(x)                                                   \
  GENERIC(x, IDENTITY, TRAIT_ORD, TYPESET(PRIMITIVE), TYPESET_CONTAINER)

#define GENERIC_ARRAY(x) GENERIC(x, IDENTITY, trait, TYPESET_ARRAY)

#define GENERIC_LIST(x) GENERIC(x, IDENTITY, trait, TYPESET_LIST)

#define TRAIT_SLICE(C) trait(Slice(C))
#define GENERIC_SLICE(x)                                                 \
  GENERIC(x, IDENTITY, TRAIT_SLICE, TYPESET_SLICEABLE)

#define GENERIC_CONTAINER(x)                                             \
  GENERIC(x, IDENTITY, trait, TYPESET_CONTAINER,                         \
          APPLY(Slice, TYPESET_SLICEABLE), APPLY(Itr, TYPESET_ITERABLE))

#define GENERIC_BOXED_CONTAINER(x)                                       \
  GENERIC(x, IDENTITY, trait, TYPESET_SLICEABLE)

#define GENERIC_FINITE_SEQUENCE(x)                                       \
  GENERIC(x, IDENTITY, trait, TYPESET_CONTAINER,                         \
          APPLY(Slice, TYPESET_SLICEABLE))

#define TRAIT_ITERABLE(C) trait(Iterable(C))
#define GENERIC_ITERABLE(x)                                              \
  GENERIC(x, IDENTITY, TRAIT_ITERABLE, TYPESET_ITERABLE,                 \
          APPLY(Itr, TYPESET_ITERABLE))

#define GENERIC_ITR(x)                                                   \
  GENERIC(x, IDENTITY, trait, APPLY(Itr, TYPESET_ITERABLE))

// clang-format on -------------------------------------------------------

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

#define g_null(x) GENERIC_CONTAINER(x).null(x)

#define g_length(x) GENERIC_FINITE_SEQUENCE(x).length(x)
#define g_reverse(x) GENERIC_FINITE_SEQUENCE(x).reverse(&(x))

#define g_free(x) GENERIC_BOXED_CONTAINER(x).free(&(x))

#define g_itr(x) GENERIC_ITERABLE(x).itr(x)

#define g_next(it) GENERIC_ITR(it).next(it)
#define g_skip(n, it) GENERIC_ITR(it).skip(n, it)
#define g_get(it) GENERIC_ITR(it).get(it)
#define g_set(v, it) GENERIC_ITR(it).set(v, it)

#define g_begin(x) GENERIC_ARRAY(x).begin(x)
#define g_end(x) GENERIC_ARRAY(x).end(x)

#define g_cons(x, xs) GENERIC_LIST(xs).cons(x, xs)
#define g_head(xs) GENERIC_LIST(xs).head(xs)
#define g_tail(xs) GENERIC_LIST(xs).tail(xs)
#define g_drop(n, xs) GENERIC_LIST(xs).drop(n, xs)

#define g_array(T, ...)                                                  \
  trait(Array(T)).from_array(VARIADIC_SIZE(__VA_ARGS__),                 \
                             (T[]){__VA_ARGS__})

#define g_list(T, ...)                                                   \
  trait(List(T)).from_array(VARIADIC_SIZE(__VA_ARGS__),                  \
                            (T[]){__VA_ARGS__})

#define g_slice(c, ...)                                                  \
  CAT(g_slice, VARIADIC_SIZE(__VA_ARGS__))(c, __VA_ARGS__)
#define g_slice1(c, s2) GENERIC_SLICE(c).slice(c, 0, s2)
#define g_slice2(c, s1, s2) GENERIC_SLICE(c).slice(c, s1, s2)

#if defined(__GNUC__)

#define g_for(it, ...)                                                   \
  CAT(g_for, VARIADIC_SIZE(__VA_ARGS__))(it, __VA_ARGS__)

#define g_for1(it, c)                                                    \
  for (__auto_type it = g_itr(c); !g_null(it); it = g_next(it))

#define g_for2(it, c, step)                                              \
  assert(step && "g_for(it, c, step): step == 0 not permitted.");        \
  for (__auto_type it = g_itr(c); !g_null(it); it = g_skip(step, it))

// ---- Structured bindings
#define g_bind(x, t) g_bind_I(t, DISCLOSE(x))
#define g_bind_I(t, ...)                                                 \
  FOREACH(EXPAND,                                                        \
          SQUASH(APPLY(g_bind0, CAT(g_bind, VARIADIC_SIZE(__VA_ARGS__))( \
                                    (t), __VA_ARGS__))))
#define g_bind0(var_, val_)                                              \
  IF(IS_NULL(var_))(, __auto_type var_ __attribute__((unused)) = val_)
// clang-format off
#define g_bind1(t, _1)                                                   \
  (_1, t.e1)
#define g_bind2(t, _1, _2)                                               \
  (_1, t.e1), (_2, t.e2)
#define g_bind3(t, _1, _2, _3)                                           \
  (_1, t.e1), (_2, t.e2), (_3, t.e3)
#define g_bind4(t, _1, _2, _3, _4)                                       \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4)
#define g_bind5(t, _1, _2, _3, _4, _5)                                   \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4), (_5, t.e5)
#define g_bind6(t, _1, _2, _3, _4, _5, _6)                               \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4), (_5, t.e5),            \
  (_6, t.e6)
#define g_bind7(t, _1, _2, _3, _4, _5, _6, _7)                           \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4), (_5, t.e5),            \
  (_6, t.e6), (_7, t.e7)
#define g_bind8(t, _1, _2, _3, _4, _5, _6, _7, _8)                       \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4), (_5, t.e5),            \
  (_6, t.e6), (_7, t.e7), (_8, t.e8)
#define g_bind9(t, _1, _2, _3, _4, _5, _6, _7, _8, _9)                   \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4), (_5, t.e5),            \
  (_6, t.e6), (_7, t.e7), (_8, t.e8), (_9, t.e9)
// clang-format on

#define TMPID CAT(tmpid__, __LINE__)

// ---- RAII
#define g_scoped(C)                                                      \
  extern void FUNC_NAME(free, C)(C*);                                    \
  __attribute__((cleanup(FUNC_NAME(free, C)))) C

#endif
