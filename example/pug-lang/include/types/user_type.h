/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

// -----------------------------------------------------------------------
#define decl_user_type(T)                                                \
  trait_Show(T);                                                         \
  trait_Mem(T);                                                          \
  trait_Array(T);                                                        \
  trait_Show(Array(T));                                                  \
  trait_Maybe(T);                                                        \
  trait_Maybe(Array(T));                                                 \
  trait_Show(Maybe(T));                                                  \
  END_OF_STATEMENTS

#define impl_user_type(T)                                                \
  static void FUNC_NAME(toString, Show(T))(CharBuff * b, T x);           \
  instance_Show(T, FUNC_NAME(toString, Show(T)));                        \
  impl_Mem(T);                                                           \
  impl_Array(T);                                                         \
  impl_Show_Array(T);                                                    \
  impl_Show_Maybe(T);                                                    \
  END_OF_STATEMENTS

#define show_user_type(T) static void FUNC_NAME(toString, Show(T))
