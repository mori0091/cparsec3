/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

/* not implemented yet */

typedef struct ClassEnv ClassEnv;
struct ClassEnv {
  None _;
};

static inline ClassEnv initialEnv(void) {
  return (ClassEnv){0};
}
