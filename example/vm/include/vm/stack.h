/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#define Stack(T) TYPE_NAME(Stack, T)

#define trait_Stack(T)                                                   \
  C_API_BEGIN                                                            \
  typedef struct Stack(T) Stack(T);                                      \
  struct Stack(T) {                                                      \
    Array(T) array; /* used + unused memory block */                     \
    size_t size;    /* size of used area */                              \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS
