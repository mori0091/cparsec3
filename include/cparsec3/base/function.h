/* -*- coding: utf-8-unix -*- */
#pragma once

#include "base.h"

#define Fn(R, ...) FnN(R, (), (DISCLOSE(__VA_ARGS__)))
#define typedef_Fn(R, ...) typedef_FnN(R, (), (DISCLOSE(__VA_ARGS__)))

#define FnN(R, BOUND, REST)                                              \
  CAT(CAT(CONCAT(Fn, DISCLOSE(BOUND)), CONCAT(, DISCLOSE(REST))),        \
      CONCAT(, R))

#define typedef_FnN(R, BOUND, REST)                                      \
  IF(IS_NULL(HEAD(DISCLOSE(BOUND))))                                     \
  (typedef_FnN0(R, REST), typedef_FnN1(R, BOUND, REST))

#define typedef_FnN0(R, REST)                                            \
  typedef R (*FnN(R, (), REST))(DISCLOSE(REST))

#define typedef_FnN1(R, BOUND, REST)                                     \
  typedef struct {                                                       \
    FnN(R, (TAIL(DISCLOSE(BOUND))),                                      \
        (SQUASH(HEAD(DISCLOSE(BOUND)), DISCLOSE(REST)))) f;              \
    HEAD(DISCLOSE(BOUND)) x;                                             \
  } FnN(R, BOUND, REST)

// typedef_FnN(int, (), (char, int, float));
// typedef_FnN(int, (char), (int, float));
// typedef_FnN(int, (int, char), (float));
// #define fapply1(f, x)
