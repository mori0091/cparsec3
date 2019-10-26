/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Result(T, E) CONCAT(Result, T, E)
#define typedef_Result(T, E)                                             \
  typedef struct {                                                       \
    bool success;                                                        \
    union {                                                              \
      T ok;                                                              \
      E err;                                                             \
    };                                                                   \
  } Result(T, E)
