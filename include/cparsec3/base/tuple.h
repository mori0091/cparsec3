/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Tuple(...) TYPE_NAME(Tuple, __VA_ARGS__)
#define typedef_Tuple(...)                                               \
  CAT(typedef_Tuple, VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)
#define typedef_Tuple1(T1)                                               \
  C_API_BEGIN                                                            \
  typedef struct Tuple(T1) Tuple(T1);                                    \
  struct Tuple(T1) {                                                     \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define typedef_Tuple2(T1, T2)                                           \
  C_API_BEGIN                                                            \
  typedef struct Tuple(T1, T2) Tuple(T1, T2);                            \
  struct Tuple(T1, T2) {                                                 \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
    union {                                                              \
      T2 e2, second;                                                     \
    };                                                                   \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define typedef_Tuple3(T1, T2, T3)                                       \
  C_API_BEGIN                                                            \
  typedef struct Tuple(T1, T2, T3) Tuple(T1, T2, T3);                    \
  struct Tuple(T1, T2, T3) {                                             \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
    union {                                                              \
      T2 e2, second;                                                     \
    };                                                                   \
    union {                                                              \
      T3 e3, third;                                                      \
    };                                                                   \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define typedef_Tuple4(T1, T2, T3, T4)                                   \
  C_API_BEGIN                                                            \
  typedef struct Tuple(T1, T2, T3, T4) Tuple(T1, T2, T3, T4);            \
  struct Tuple(T1, T2, T3, T4) {                                         \
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
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define typedef_Tuple5(T1, T2, T3, T4, T5)                               \
  C_API_BEGIN                                                            \
  typedef struct Tuple(T1, T2, T3, T4, T5) Tuple(T1, T2, T3, T4, T5);    \
  struct Tuple(T1, T2, T3, T4, T5) {                                     \
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
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define typedef_Tuple6(T1, T2, T3, T4, T5, T6)                           \
  C_API_BEGIN                                                            \
  typedef struct Tuple(T1, T2, T3, T4, T5, T6)                           \
      Tuple(T1, T2, T3, T4, T5, T6);                                     \
  struct Tuple(T1, T2, T3, T4, T5, T6) {                                 \
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
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

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
  FOREACH(EXPAND,                                                        \
          SQUASH(APPLY(tie0, CAT(tie, VARIADIC_SIZE(__VA_ARGS__))(       \
                                 t, __VA_ARGS__))))
#define tie0(var_, val_) IF(IS_NULL(var_))(, var_ = val_)
// clang-format off
#define tie1(t, _1)                                                      \
  (_1, t.e1)
#define tie2(t, _1, _2)                                                  \
  (_1, t.e1), (_2, t.e2)
#define tie3(t, _1, _2, _3)                                              \
  (_1, t.e1), (_2, t.e2), (_3, t.e3)
#define tie4(t, _1, _2, _3, _4)                                          \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4)
#define tie5(t, _1, _2, _3, _4, _5)                                      \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4), (_5, t.e5)
#define tie6(t, _1, _2, _3, _4, _5, _6)                                  \
  (_1, t.e1), (_2, t.e2), (_3, t.e3), (_4, t.e4), (_5, t.e5), (_6, t.e6)
// clang-format on
