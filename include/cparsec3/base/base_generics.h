/* -*- coding: utf-8-unix -*- */
#pragma once

#include "base.h"

#define CREATE_TRAIT(...) trait(TYPE_NAME(__VA_ARGS__))

#define BIND_TYPESET(CONTAINER) BIND(CONTAINER, TYPESET_COMPONENT)
#define APPLY_TYPESET(CONTAINER) APPLY(CONTAINER, TYPESET_COMPONENT)

// clang-format off ------------------------------------------------------
#define GENERIC_EQ(x)                                                    \
  GENERIC(x, SND, CREATE_TRAIT,                                          \
          BIND(Eq, TYPESET(PRIMITIVE), APPLY_TYPESET(Array),             \
               APPLY_TYPESET(List), APPLY_TYPESET(Maybe)))

#define GENERIC_ORD(x)                                                   \
  GENERIC(x, SND, CREATE_TRAIT,                                          \
          BIND(Ord, TYPESET(PRIMITIVE), APPLY_TYPESET(Array),            \
               APPLY_TYPESET(List), APPLY_TYPESET(Maybe)))

#define GENERIC_ARRAY(x)                                                 \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(Array))

#define GENERIC_LIST(x)                                                  \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(List))

#define GENERIC_SLICE(x)                                                 \
  GENERIC(x, SND, CREATE_TRAIT,                                          \
          BIND(Slice, APPLY_TYPESET(Array), APPLY_TYPESET(List)))

#define GENERIC_CONTAINER(x)                                             \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(Array),               \
          BIND_TYPESET(List), BIND_TYPESET(Maybe),                       \
          BIND(Slice, APPLY_TYPESET(Array), APPLY_TYPESET(List)),        \
          BIND(Itr, APPLY_TYPESET(Array), APPLY_TYPESET(List),           \
               APPLY(Slice, APPLY_TYPESET(Array), APPLY_TYPESET(List))))

#define GENERIC_BOXED_CONTAINER(x)                                       \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(Array),               \
          BIND_TYPESET(List))

#define GENERIC_FINITE_SEQUENCE(x)                                       \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT, BIND_TYPESET(Array),               \
          BIND_TYPESET(List), BIND_TYPESET(Maybe),                       \
          BIND(Slice, APPLY_TYPESET(Array), APPLY_TYPESET(List)))

#define GENERIC_ITERABLE(x)                                              \
  GENERIC(x, SND, CREATE_TRAIT,                                          \
          BIND(Itr, APPLY_TYPESET(Array), APPLY_TYPESET(List),           \
               APPLY(Slice, APPLY_TYPESET(Array), APPLY_TYPESET(List)),  \
               APPLY(Itr, APPLY_TYPESET(Array), APPLY_TYPESET(List),     \
                     APPLY(Slice, APPLY_TYPESET(Array),                  \
                           APPLY_TYPESET(List)))))

#define GENERIC_ITR(x)                                                   \
  GENERIC(x, TYPE_NAME, CREATE_TRAIT,                                    \
          BIND(Itr, APPLY_TYPESET(Array), APPLY_TYPESET(List),           \
               APPLY(Slice, APPLY_TYPESET(Array), APPLY_TYPESET(List))))
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
  FOREACH(EXPAND, SQUASH(APPLY(g_bind0, (TMPID, t),                      \
                               CAT(g_bind, VARIADIC_SIZE(__VA_ARGS__))(  \
                                   TMPID, __VA_ARGS__))))
#define g_bind0(var_, val_) IF(IS_NULL(var_))(, __auto_type var_ = val_)
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
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4), (_5, t.e5), (_6, t.e6)
// clang-format on

#define TMPID CAT(tmpid__, __LINE__)

// ---- RAII
#define g_scoped(C)                                                      \
  extern void FUNC_NAME(free, C)(C*);                                    \
  __attribute__((cleanup(FUNC_NAME(free, C)))) C

#endif
