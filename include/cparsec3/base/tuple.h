/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Tuple(...) TYPE_NAME(Tuple, __VA_ARGS__)
#define typedef_Tuple(...)                                               \
  CAT(typedef_Tuple, VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)
#define typedef_Tuple1(T1)                                               \
  typedef struct {                                                       \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
  } Tuple(T1)
#define typedef_Tuple2(T1, T2)                                           \
  typedef struct {                                                       \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
    union {                                                              \
      T2 e2, second;                                                     \
    };                                                                   \
  } Tuple(T1, T2)
#define typedef_Tuple3(T1, T2, T3)                                       \
  typedef struct {                                                       \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
    union {                                                              \
      T2 e2, second;                                                     \
    };                                                                   \
    union {                                                              \
      T3 e3, third;                                                      \
    };                                                                   \
  } Tuple(T1, T2, T3)
#define typedef_Tuple4(T1, T2, T3, T4)                                   \
  typedef struct {                                                       \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
    union {                                                              \
      T2 e2, second;                                                     \
    };                                                                   \
    union {                                                              \
      T3 e3, third;                                                      \
    };                                                                   \
    union {                                                              \
      T4 e4, fourth;                                                     \
    };                                                                   \
  } Tuple(T1, T2, T3, T4)
#define typedef_Tuple5(T1, T2, T3, T4, T5)                               \
  typedef struct {                                                       \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
    union {                                                              \
      T2 e2, second;                                                     \
    };                                                                   \
    union {                                                              \
      T3 e3, third;                                                      \
    };                                                                   \
    union {                                                              \
      T4 e4, fourth;                                                     \
    };                                                                   \
    union {                                                              \
      T5 e5, fifth;                                                      \
    };                                                                   \
  } Tuple(T1, T2, T3, T4, T5)
#define typedef_Tuple6(T1, T2, T3, T4, T5, T6)                           \
  typedef struct {                                                       \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
    union {                                                              \
      T2 e2, second;                                                     \
    };                                                                   \
    union {                                                              \
      T3 e3, third;                                                      \
    };                                                                   \
    union {                                                              \
      T4 e4, fourth;                                                     \
    };                                                                   \
    union {                                                              \
      T5 e5, fifth;                                                      \
    };                                                                   \
    union {                                                              \
      T6 e6, sixth;                                                      \
    };                                                                   \
  } Tuple(T1, T2, T3, T4, T5, T6)

// -----------------------------------------------------------------------
/**
 * \brief tie((v1, v2, ...), t) - Maps tuple t to (v1, v2, ...)
 *
 * tie((x,y), t)   -> x = t.e1; y = t.e2
 * tie((x, ), t)   -> x = t.e1
 * tie(( ,y), t)   -> y = t.e2
 * tie((x, ,z), t) -> x = t.e1; z = t.e3
 */
#define tie(x, t) tie_I(t, DISCLOSE(x))
#define tie_I(t, ...)                                                    \
  CAT(tie, VARIADIC_SIZE(__VA_ARGS__))                                   \
  (t, __VA_ARGS__)
// clang-format off
#define tie1(t, _1)                             \
  tie0((_1, _1 = t.e1))
#define tie2(t, _1, _2)                         \
  tie0((_1, _1 = t.e1),                         \
       (_2, _2 = t.e2))
#define tie3(t, _1, _2, _3)                     \
  tie0((_1, _1 = t.e1),                         \
       (_2, _2 = t.e2),                         \
       (_3, _3 = t.e3))
#define tie4(t, _1, _2, _3, _4)                 \
  tie0((_1, _1 = t.e1),                         \
       (_2, _2 = t.e2),                         \
       (_3, _3 = t.e3),                         \
       (_4, _4 = t.e4))
#define tie5(t, _1, _2, _3, _4, _5)             \
  tie0((_1, _1 = t.e1),                         \
       (_2, _2 = t.e2),                         \
       (_3, _3 = t.e3),                         \
       (_4, _4 = t.e4),                         \
       (_5, _5 = t.e5))
#define tie6(t, _1, _2, _3, _4, _5, _6)         \
  tie0((_1, _1 = t.e1),                         \
       (_2, _2 = t.e2),                         \
       (_3, _3 = t.e3),                         \
       (_4, _4 = t.e4),                         \
       (_5, _5 = t.e5),                         \
       (_6, _6 = t.e6))
// clang-format on
#define tie0(...) FOREACH(tie0_id, SQUASH(APPLY(tie0_f, __VA_ARGS__)))
#define tie0_f(x, y) IF(IS_NULL(x))(, y)
#define tie0_id(x) x
