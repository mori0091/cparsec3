/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Maybe(T) CONCAT(Maybe, T)
#define typedef_Maybe(T)                                                 \
  typedef struct {                                                       \
    bool none;                                                           \
    T value;                                                             \
  } Maybe(T)
