/* -*- coding:utf-8-unix -*- */
#pragma once

#include "common.h"

C_API_BEGIN

typedef struct None None;
struct None {
  char _;
};

static const None NONE = {0};

C_API_END

#ifdef __cplusplus

template <typename T>
inline auto isNONE(T) {
  return false;
}
template <>
inline auto isNONE(None) {
  return true;
}

#else

#define isNONE(x) (_Generic((x), None : 1, default : 0))

#endif
