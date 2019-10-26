/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Tupple(...) CONCAT(Tupple, __VA_ARGS__)
#define typedef_Tupple(...)                                              \
  CAT(typedef_Tupple, VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)
#define typedef_Tupple1(T1)                                              \
  typedef struct {                                                       \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
  } Tupple(T1)
#define typedef_Tupple2(T1, T2)                                          \
  typedef struct {                                                       \
    union {                                                              \
      T1 e1, first;                                                      \
    };                                                                   \
    union {                                                              \
      T2 e2, second;                                                     \
    };                                                                   \
  } Tupple(T1, T2)
#define typedef_Tupple3(T1, T2, T3)                                      \
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
  } Tupple(T1, T2, T3)
#define typedef_Tupple4(T1, T2, T3, T4)                                  \
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
  } Tupple(T1, T2, T3, T4)
#define typedef_Tupple5(T1, T2, T3, T4, T5)                              \
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
  } Tupple(T1, T2, T3, T4, T5)
#define typedef_Tupple6(T1, T2, T3, T4, T5, T6)                          \
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
  } Tupple(T1, T2, T3, T4, T5, T6)
