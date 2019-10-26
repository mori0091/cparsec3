/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Array(T) CONCAT(Array, T)
#define typedef_Array(T)                                                 \
  typedef struct {                                                       \
    size_t length;                                                       \
    T* data;                                                             \
  } Array(T)
