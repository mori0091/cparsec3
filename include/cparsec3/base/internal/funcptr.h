/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../common.h"

// -----------------------------------------------------------------------
#undef Fp
#define Fp(...) Fp0(__VA_ARGS__)
#define Fp0(T, ...) FOLDR(Fp1, T, __VA_ARGS__)
#define Fp1(T, R) TYPE_NAME(Fp, T, R)

// -----------------------------------------------------------------------
#undef typedef_Fp
#define typedef_Fp(...) typedef_Fp0(__VA_ARGS__)
#define typedef_Fp0(T, ...)                                              \
  CAT(typedef_Fp, VARIADIC_SIZE(__VA_ARGS__))(T, __VA_ARGS__)

#define typedef_Fp1(T1, R) typedef R (*Fp(T1, R))(T1)
#define typedef_Fp2(T1, T2, R) typedef R (*Fp(T1, T2, R))(T1, T2)
#define typedef_Fp3(T1, T2, T3, R)                                       \
  typedef R (*Fp(T1, T2, T3, R))(T1, T2, T3)
#define typedef_Fp4(T1, T2, T3, T4, R)                                   \
  typedef R (*Fp(T1, T2, T3, T4, R))(T1, T2, T3, T4)
#define typedef_Fp5(T1, T2, T3, T4, T5, R)                               \
  typedef R (*Fp(T1, T2, T3, T4, T5, R))(T1, T2, T3, T4, T5)
#define typedef_Fp6(T1, T2, T3, T4, T5, T6, R)                           \
  typedef R (*Fp(T1, T2, T3, T4, T5, T6, R))(T1, T2, T3, T4, T5, T6)
#define typedef_Fp7(T1, T2, T3, T4, T5, T6, T7, R)                       \
  typedef R (*Fp(T1, T2, T3, T4, T5, T6, T7, R))(T1, T2, T3, T4, T5, T6, \
                                                 T7)
#define typedef_Fp8(T1, T2, T3, T4, T5, T6, T7, T8, R)                   \
  typedef R (*Fp(T1, T2, T3, T4, T5, T6, T7, T8, R))(T1, T2, T3, T4, T5, \
                                                     T6, T7, T8)
#define typedef_Fp9(T1, T2, T3, T4, T5, T6, T7, T8, T9, R)               \
  typedef R (*Fp(T1, T2, T3, T4, T5, T6, T7, T8, T9, R))(                \
      T1, T2, T3, T4, T5, T6, T7, T8, T9)

// -----------------------------------------------------------------------
#undef fp
#define fp(name, ...) fp0(name, __VA_ARGS__)
#define fp0(name, T, ...)                                                \
  CAT(fp, VARIADIC_SIZE(__VA_ARGS__))(name, T, __VA_ARGS__)

#define fp1(name, T1, R)                                                 \
  typedef_Fp(T1, R);                                                     \
  typedef_Fn(Fp(T1, R), T1, R);                                          \
  fn(fp_##name, Fp(T1, R), T1, R) {                                      \
    g_bind((fp_, t1), *args);                                            \
    return (*fp_)(t1);                                                   \
  }                                                                      \
  static Fn(T1, R) name(Fp(T1, R) fp_) {                                 \
    Fn(Fp(T1, R), T1, R) f = fp_##name();                                \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS

#define fp2(name, T1, T2, R)                                             \
  typedef_Fp(T1, T2, R);                                                 \
  typedef_Fn(Fp(T1, T2, R), T1, T2, R);                                  \
  fn(fp_##name, Fp(T1, T2, R), T1, T2, R) {                              \
    g_bind((fp_, t1, t2), *args);                                        \
    return (*fp_)(t1, t2);                                               \
  }                                                                      \
  static Fn(T1, T2, R) name(Fp(T1, T2, R) fp_) {                         \
    Fn(Fp(T1, T2, R), T1, T2, R) f = fp_##name();                        \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS

#define fp3(name, T1, T2, T3, R)                                         \
  typedef_Fp(T1, T2, T3, R);                                             \
  typedef_Fn(Fp(T1, T2, T3, R), T1, T2, T3, R);                          \
  fn(fp_##name, Fp(T1, T2, T3, R), T1, T2, T3, R) {                      \
    g_bind((fp_, t1, t2, t3), *args);                                    \
    return (*fp_)(t1, t2, t3);                                           \
  }                                                                      \
  static Fn(T1, T2, T3, R) name(Fp(T1, T2, T3, R) fp_) {                 \
    Fn(Fp(T1, T2, T3, R), T1, T2, T3, R) f = fp_##name();                \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS

#define fp4(name, T1, T2, T3, T4, R)                                     \
  typedef_Fp(T1, T2, T3, T4, R);                                         \
  typedef_Fn(Fp(T1, T2, T3, T4, R), T1, T2, T3, T4, R);                  \
  fn(fp_##name, Fp(T1, T2, T3, T4, R), T1, T2, T3, T4, R) {              \
    g_bind((fp_, t1, t2, t3, t4), *args);                                \
    return (*fp_)(t1, t2, t3, t4);                                       \
  }                                                                      \
  static Fn(T1, T2, T3, T4, R) name(Fp(T1, T2, T3, T4, R) fp_) {         \
    Fn(Fp(T1, T2, T3, T4, R), T1, T2, T3, T4, R) f = fp_##name();        \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS

#define fp5(name, T1, T2, T3, T4, T5, R)                                 \
  typedef_Fp(T1, T2, T3, T4, T5, R);                                     \
  typedef_Fn(Fp(T1, T2, T3, T4, T5, R), T1, T2, T3, T4, T5, R);          \
  fn(fp_##name, Fp(T1, T2, T3, T4, T5, R), T1, T2, T3, T4, T5, R) {      \
    g_bind((fp_, t1, t2, t3, t4, t5), *args);                            \
    return (*fp_)(t1, t2, t3, t4, t5);                                   \
  }                                                                      \
  static Fn(T1, T2, T3, T4, T5, R) name(Fp(T1, T2, T3, T4, T5, R) fp_) { \
    Fn(Fp(T1, T2, T3, T4, T5, R), T1, T2, T3, T4, T5, R) f =             \
        fp_##name();                                                     \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS

#define fp6(name, T1, T2, T3, T4, T5, T6, R)                             \
  typedef_Fp(T1, T2, T3, T4, T5, T6, R);                                 \
  typedef_Fn(Fp(T1, T2, T3, T4, T5, T6, R), T1, T2, T3, T4, T5, T6, R);  \
  fn(fp_##name, Fp(T1, T2, T3, T4, T5, T6, R), T1, T2, T3, T4, T5, T6,   \
     R) {                                                                \
    g_bind((fp_, t1, t2, t3, t4, t5, t6), *args);                        \
    return (*fp_)(t1, t2, t3, t4, t5, t6);                               \
  }                                                                      \
  static Fn(T1, T2, T3, T4, T5, T6, R)                                   \
      name(Fp(T1, T2, T3, T4, T5, T6, R) fp_) {                          \
    Fn(Fp(T1, T2, T3, T4, T5, T6, R), T1, T2, T3, T4, T5, T6, R) f =     \
        fp_##name();                                                     \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS

#define fp7(name, T1, T2, T3, T4, T5, T6, T7, R)                         \
  typedef_Fp(T1, T2, T3, T4, T5, T6, T7, R);                             \
  typedef_Fn(Fp(T1, T2, T3, T4, T5, T6, T7, R), T1, T2, T3, T4, T5, T6,  \
             T7, R);                                                     \
  fn(fp_##name, Fp(T1, T2, T3, T4, T5, T6, T7, R), T1, T2, T3, T4, T5,   \
     T6, T7, R) {                                                        \
    g_bind((fp_, t1, t2, t3, t4, t5, t6, t7), *args);                    \
    return (*fp_)(t1, t2, t3, t4, t5, t6, t7);                           \
  }                                                                      \
  static Fn(T1, T2, T3, T4, T5, T6, T7, R)                               \
      name(Fp(T1, T2, T3, T4, T5, T6, T7, R) fp_) {                      \
    Fn(Fp(T1, T2, T3, T4, T5, T6, T7, R), T1, T2, T3, T4, T5, T6, T7, R) \
        f = fp_##name();                                                 \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS

#define fp8(name, T1, T2, T3, T4, T5, T6, T7, T8, R)                     \
  typedef_Fp(T1, T2, T3, T4, T5, T6, T7, T8, R);                         \
  typedef_Fn(Fp(T1, T2, T3, T4, T5, T6, T7, T8, R), T1, T2, T3, T4, T5,  \
             T6, T7, T8, R);                                             \
  fn(fp_##name, Fp(T1, T2, T3, T4, T5, T6, T7, T8, R), T1, T2, T3, T4,   \
     T5, T6, T7, T8, R) {                                                \
    g_bind((fp_, t1, t2, t3, t4, t5, t6, t7, t8), *args);                \
    return (*fp_)(t1, t2, t3, t4, t5, t6, t7, t8);                       \
  }                                                                      \
  static Fn(T1, T2, T3, T4, T5, T6, T7, T8, R)                           \
      name(Fp(T1, T2, T3, T4, T5, T6, T7, T8, R) fp_) {                  \
    Fn(Fp(T1, T2, T3, T4, T5, T6, T7, T8, R), T1, T2, T3, T4, T5, T6,    \
       T7, T8, R) f = fp_##name();                                       \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS

#define fp9(name, T1, T2, T3, T4, T5, T6, T7, T8, T9, R)                 \
  typedef_Fp(T1, T2, T3, T4, T5, T6, T7, T8, T9, R);                     \
  typedef_Fn(Fp(T1, T2, T3, T4, T5, T6, T7, T8, T9, R), T1, T2, T3, T4,  \
             T5, T6, T7, T8, T9, R);                                     \
  fn(fp_##name, Fp(T1, T2, T3, T4, T5, T6, T7, T8, T9, R), T1, T2, T3,   \
     T4, T5, T6, T7, T8, T9, R) {                                        \
    g_bind((fp_, t1, t2, t3, t4, t5, t6, t7, t8, t9), *args);            \
    return (*fp_)(t1, t2, t3, t4, t5, t6, t7, t8, t9);                   \
  }                                                                      \
  static Fn(T1, T2, T3, T4, T5, T6, T7, T8, T9, R)                       \
      name(Fp(T1, T2, T3, T4, T5, T6, T7, T8, T9, R) fp_) {              \
    Fn(Fp(T1, T2, T3, T4, T5, T6, T7, T8, T9, R), T1, T2, T3, T4, T5,    \
       T6, T7, T8, T9, R) f = fp_##name();                               \
    return fn_apply(f, fp_);                                             \
  }                                                                      \
  END_OF_STATEMENTS
