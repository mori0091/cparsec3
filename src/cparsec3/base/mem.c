/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

// -----------------------------------------------------------------------
#define impl_Mem(T)                                                      \
  C_API_BEGIN                                                            \
  static T* FUNC_NAME(create, T)(size_t n) {                             \
    return (T*)malloc(n * sizeof(T));                                    \
  }                                                                      \
  static T* FUNC_NAME(recreate, T)(T * ptr, size_t n) {                  \
    return (T*)realloc(ptr, n * sizeof(T));                              \
  }                                                                      \
  static void FUNC_NAME(free, T)(T * p) {                                \
    free((void*)p);                                                      \
  }                                                                      \
  Mem(T) Trait(Mem(T)) {                                                 \
    return (Mem(T)){.create = FUNC_NAME(create, T),                      \
                    .recreate = FUNC_NAME(recreate, T),                  \
                    .free = FUNC_NAME(free, T)};                         \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
FOREACH(impl_Mem, TYPESET(ALL));
