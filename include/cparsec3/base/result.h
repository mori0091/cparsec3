/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Result(T, E) TYPE_NAME(Result, T, E)
#define typedef_Result(T, E)                                             \
  C_API_BEGIN                                                            \
  typedef struct Result(T, E) Result(T, E);                              \
  struct Result(T, E) {                                                  \
    bool success;                                                        \
    union {                                                              \
      T ok;                                                              \
      E err;                                                             \
    };                                                                   \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS
