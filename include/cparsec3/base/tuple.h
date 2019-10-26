/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Tuple(...) CONCAT(Tuple, __VA_ARGS__)
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
