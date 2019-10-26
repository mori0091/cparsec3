/* -*- coding: utf-8-unix -*- */
#pragma once

// ---- True if the type of expr was T, false otherwise
#ifdef __cplusplus
#include <type_traits>

#define type_eq(T, expr) (std::is_same<T, decltype(expr)>::value)

#define is_same(T1, T2) (std::is_same<T1, T2>::value)

#define is_type(T) (is_same(T, T))

#else

#define type_eq(T, expr) (_Generic((expr), T : 1, default : 0))

#define is_same(T1, T2) (is_same0(T1, T2) && is_same0(T2, T1))
#define is_same0(T1, T2) _Generic(((T1){0}), T2: 1, default: 0)

#define is_type(T)                                            \
  _Generic((void (*)(int (T))), void (*)(int): 0, default 1)

#endif
