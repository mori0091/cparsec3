/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../common.h"
#include "../mem.h"

// -----------------------------------------------------------------------
#undef Fn
#define Fn(...) Fn0(__VA_ARGS__)
#define Fn0(T, ...) FOLDR(Fn1, T, __VA_ARGS__)
#define Fn1(T, R) TYPE_NAME(Fn, T, R)

// -----------------------------------------------------------------------
#undef fn_apply
#define fn_apply(...) fn_apply0(__VA_ARGS__)
#define fn_apply0(f, ...)                                                \
  FOLDL(fn_apply1, (f), BIND(((f).args), __VA_ARGS__))
#define fn_apply1(f, ax) f.apply ax

// -----------------------------------------------------------------------
#undef typedef_Fn
#define typedef_Fn(...) typedef_Fn0(__VA_ARGS__)

#define typedef_Fn0(T, ...)                                              \
  IF(IS_NIL(TAIL(__VA_ARGS__, )))                                        \
  (typedef_Fn1, typedef_FnN)(T, __VA_ARGS__)

#define typedef_Fn1(T1, R)                                               \
  C_API_BEGIN                                                            \
  typedef struct Fn(T1, R) Fn(T1, R);                                    \
  struct Fn(T1, R) {                                                     \
    void* args;                                                          \
    R (*apply)(void* args, T1 x);                                        \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define typedef_FnN(...)                                                 \
  C_API_BEGIN                                                            \
  typedef struct Fn(__VA_ARGS__) Fn(__VA_ARGS__);                        \
  struct Fn(__VA_ARGS__) {                                               \
    void* args;                                                          \
    Fn(TAIL(__VA_ARGS__)) (*apply)(void* args, HEAD(__VA_ARGS__) x);     \
  };                                                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#undef typedef_Fn_r
#define typedef_Fn_r(...) typedef_Fn_r0(__VA_ARGS__)

#define typedef_Fn_r0(T, ...)                                            \
  CAT(typedef_Fn_r, VARIADIC_SIZE(__VA_ARGS__))(T, __VA_ARGS__)

#define typedef_Fn_r1(T1, R) typedef_Fn1(T1, R)
#define typedef_Fn_r2(T2, T1, R)                                         \
  typedef_Fn1(T1, R);                                                    \
  typedef_FnN(T2, T1, R)
#define typedef_Fn_r3(T3, T2, T1, R)                                     \
  typedef_Fn_r2(T2, T1, R);                                              \
  typedef_FnN(T3, T2, T1, R)
#define typedef_Fn_r4(T4, T3, T2, T1, R)                                 \
  typedef_Fn_r3(T3, T2, T1, R);                                          \
  typedef_FnN(T4, T3, T2, T1, R)
#define typedef_Fn_r5(T5, T4, T3, T2, T1, R)                             \
  typedef_Fn_r4(T4, T3, T2, T1, R);                                      \
  typedef_FnN(T5, T4, T3, T2, T1, R)
#define typedef_Fn_r6(T6, T5, T4, T3, T2, T1, R)                         \
  typedef_Fn_r5(T5, T4, T3, T2, T1, R);                                  \
  typedef_FnN(T6, T5, T4, T3, T2, T1, R)
#define typedef_Fn_r7(T7, T6, T5, T4, T3, T2, T1, R)                     \
  typedef_Fn_r6(T6, T5, T4, T3, T2, T1, R);                              \
  typedef_FnN(T7, T6, T5, T4, T3, T2, T1, R)
#define typedef_Fn_r8(T8, T7, T6, T5, T4, T3, T2, T1, R)                 \
  typedef_Fn_r7(T7, T6, T5, T4, T3, T2, T1, R);                          \
  typedef_FnN(T8, T7, T6, T5, T4, T3, T2, T1, R)
#define typedef_Fn_r9(T9, T8, T7, T6, T5, T4, T3, T2, T1, R)             \
  typedef_Fn_r8(T8, T7, T6, T5, T4, T3, T2, T1, R);                      \
  typedef_FnN(T9, T8, T7, T6, T5, T4, T3, T2, T1, R)

// -----------------------------------------------------------------------
#define FnEnv(tag, ...) FnEnv0(tag, __LINE__, __VA_ARGS__)
#define FnEnv0(tag, line, ...)                                           \
  TYPE_NAME(FnEnv_##line##_##tag, __VA_ARGS__)

#define typedef_FnEnv(tag, ...) typedef_FnEnv0(tag, __VA_ARGS__)

#define typedef_FnEnv0(tag, ...)                                         \
  CAT(typedef_FnEnv, VARIADIC_SIZE(__VA_ARGS__))(tag, __VA_ARGS__)

#define typedef_FnEnv1(tag, T1)                                          \
  typedef struct {                                                       \
    T1 e1;                                                               \
  } FnEnv(tag, T1)

#define typedef_FnEnv2(tag, T1, T2)                                      \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
  } FnEnv(tag, T1, T2)

#define typedef_FnEnv3(tag, T1, T2, T3)                                  \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
  } FnEnv(tag, T1, T2, T3)

#define typedef_FnEnv4(tag, T1, T2, T3, T4)                              \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
    T4 e4;                                                               \
  } FnEnv(tag, T1, T2, T3, T4)

#define typedef_FnEnv5(tag, T1, T2, T3, T4, T5)                          \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
    T4 e4;                                                               \
    T5 e5;                                                               \
  } FnEnv(tag, T1, T2, T3, T4, T5)

#define typedef_FnEnv6(tag, T1, T2, T3, T4, T5, T6)                      \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
    T4 e4;                                                               \
    T5 e5;                                                               \
    T6 e6;                                                               \
  } FnEnv(tag, T1, T2, T3, T4, T5, T6)

#define typedef_FnEnv7(tag, T1, T2, T3, T4, T5, T6, T7)                  \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
    T4 e4;                                                               \
    T5 e5;                                                               \
    T6 e6;                                                               \
    T7 e7;                                                               \
  } FnEnv(tag, T1, T2, T3, T4, T5, T6, T7)

#define typedef_FnEnv8(tag, T1, T2, T3, T4, T5, T6, T7, T8)              \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
    T4 e4;                                                               \
    T5 e5;                                                               \
    T6 e6;                                                               \
    T7 e7;                                                               \
    T8 e8;                                                               \
  } FnEnv(tag, T1, T2, T3, T4, T5, T6, T7, T8)

#define typedef_FnEnv9(tag, T1, T2, T3, T4, T5, T6, T7, T8, T9)          \
  typedef struct {                                                       \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
    T4 e4;                                                               \
    T5 e5;                                                               \
    T6 e6;                                                               \
    T7 e7;                                                               \
    T8 e8;                                                               \
    T9 e9;                                                               \
  } FnEnv(tag, T1, T2, T3, T4, T5, T6, T7, T8, T9)

// -----------------------------------------------------------------------
#undef fn
#define fn(...) fn0(__VA_ARGS__)
#define fn0(name, T, ...)                                                \
  CAT(fn, VARIADIC_SIZE(__VA_ARGS__))(name, T, __VA_ARGS__)

// -----------------------------------------------------------------------
#define fn1(name, T1, R)                                                 \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T1);                                               \
  /* ---- */                                                             \
  static Fn(T1, R) name(void);                                           \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T1) * args);              \
  /* ---- */                                                             \
  static Fn(T1, R) name(void) {                                          \
    return (Fn(T1, R)){                                                  \
        .args = mem_malloc(sizeof(FnEnv(name, T1))),                     \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T1)* t = args;                                           \
    t->e1 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T1) * args)

// -----------------------------------------------------------------------
#define fn2(name, T2, T1, R)                                             \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T2, T1);                                           \
  /* ---- */                                                             \
  static Fn(T2, T1, R) name(void);                                       \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x);            \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T2, T1) * args);          \
  /* ---- */                                                             \
  static Fn(T2, T1, R) name(void) {                                      \
    return (Fn(T2, T1, R)){                                              \
        .args = mem_malloc(sizeof(FnEnv(name, T2, T1))),                 \
        .apply = FUNC_NAME(name, apply2),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x) {           \
    FnEnv(name, T2, T1)* t = args;                                       \
    t->e1 = x;                                                           \
    return (Fn(T1, R)){                                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T2, T1)* t = args;                                       \
    t->e2 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T2, T1) * args)

// -----------------------------------------------------------------------
#define fn3(name, T3, T2, T1, R)                                         \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T3, T2, T1);                                       \
  /* ---- */                                                             \
  static Fn(T3, T2, T1, R) name(void);                                   \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x);        \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x);            \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T3, T2, T1) * args);      \
  /* ---- */                                                             \
  static Fn(T3, T2, T1, R) name(void) {                                  \
    return (Fn(T3, T2, T1, R)){                                          \
        .args = mem_malloc(sizeof(FnEnv(name, T3, T2, T1))),             \
        .apply = FUNC_NAME(name, apply3),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x) {       \
    FnEnv(name, T3, T2, T1)* t = args;                                   \
    t->e1 = x;                                                           \
    return (Fn(T2, T1, R)){                                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply2),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x) {           \
    FnEnv(name, T3, T2, T1)* t = args;                                   \
    t->e2 = x;                                                           \
    return (Fn(T1, R)){                                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T3, T2, T1)* t = args;                                   \
    t->e3 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T3, T2, T1) * args)

// -----------------------------------------------------------------------
#define fn4(name, T4, T3, T2, T1, R)                                     \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T4, T3, T2, T1);                                   \
  /* ---- */                                                             \
  static Fn(T4, T3, T2, T1, R) name(void);                               \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x);    \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x);        \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x);            \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T4, T3, T2, T1) * args);  \
  /* ---- */                                                             \
  static Fn(T4, T3, T2, T1, R) name(void) {                              \
    return (Fn(T4, T3, T2, T1, R)){                                      \
        .args = mem_malloc(sizeof(FnEnv(name, T4, T3, T2, T1))),         \
        .apply = FUNC_NAME(name, apply4),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x) {   \
    FnEnv(name, T4, T3, T2, T1)* t = args;                               \
    t->e1 = x;                                                           \
    return (Fn(T3, T2, T1, R)){                                          \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply3),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x) {       \
    FnEnv(name, T4, T3, T2, T1)* t = args;                               \
    t->e2 = x;                                                           \
    return (Fn(T2, T1, R)){                                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply2),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x) {           \
    FnEnv(name, T4, T3, T2, T1)* t = args;                               \
    t->e3 = x;                                                           \
    return (Fn(T1, R)){                                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T4, T3, T2, T1)* t = args;                               \
    t->e4 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T4, T3, T2, T1) * args)

// -----------------------------------------------------------------------
#define fn5(name, T5, T4, T3, T2, T1, R)                                 \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T5, T4, T3, T2, T1);                               \
  /* ---- */                                                             \
  static Fn(T5, T4, T3, T2, T1, R) name(void);                           \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x);                         \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x);    \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x);        \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x);            \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name,                                               \
                     apply0)(FnEnv(name, T5, T4, T3, T2, T1) * args);    \
  /* ---- */                                                             \
  static Fn(T5, T4, T3, T2, T1, R) name(void) {                          \
    return (Fn(T5, T4, T3, T2, T1, R)){                                  \
        .args = mem_malloc(sizeof(FnEnv(name, T5, T4, T3, T2, T1))),     \
        .apply = FUNC_NAME(name, apply5),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x) {                        \
    FnEnv(name, T5, T4, T3, T2, T1)* t = args;                           \
    t->e1 = x;                                                           \
    return (Fn(T4, T3, T2, T1, R)){                                      \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply4),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x) {   \
    FnEnv(name, T5, T4, T3, T2, T1)* t = args;                           \
    t->e2 = x;                                                           \
    return (Fn(T3, T2, T1, R)){                                          \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply3),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x) {       \
    FnEnv(name, T5, T4, T3, T2, T1)* t = args;                           \
    t->e3 = x;                                                           \
    return (Fn(T2, T1, R)){                                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply2),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x) {           \
    FnEnv(name, T5, T4, T3, T2, T1)* t = args;                           \
    t->e4 = x;                                                           \
    return (Fn(T1, R)){                                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T5, T4, T3, T2, T1)* t = args;                           \
    t->e5 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T5, T4, T3, T2, T1) * args)

// -----------------------------------------------------------------------
#define fn6(name, T6, T5, T4, T3, T2, T1, R)                             \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T6, T5, T4, T3, T2, T1);                           \
  /* ---- */                                                             \
  static Fn(T6, T5, T4, T3, T2, T1, R) name(void);                       \
  static Fn(T5, T4, T3, T2, T1, R)                                       \
      FUNC_NAME(name, apply6)(void* args, T6 x);                         \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x);                         \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x);    \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x);        \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x);            \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name, apply0)(FnEnv(name, T6, T5, T4, T3, T2, T1) * \
                                   args);                                \
  /* ---- */                                                             \
  static Fn(T6, T5, T4, T3, T2, T1, R) name(void) {                      \
    return (Fn(T6, T5, T4, T3, T2, T1, R)){                              \
        .args = mem_malloc(sizeof(FnEnv(name, T6, T5, T4, T3, T2, T1))), \
        .apply = FUNC_NAME(name, apply6),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T5, T4, T3, T2, T1, R)                                       \
      FUNC_NAME(name, apply6)(void* args, T6 x) {                        \
    FnEnv(name, T6, T5, T4, T3, T2, T1)* t = args;                       \
    t->e1 = x;                                                           \
    return (Fn(T5, T4, T3, T2, T1, R)){                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply5),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x) {                        \
    FnEnv(name, T6, T5, T4, T3, T2, T1)* t = args;                       \
    t->e2 = x;                                                           \
    return (Fn(T4, T3, T2, T1, R)){                                      \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply4),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x) {   \
    FnEnv(name, T6, T5, T4, T3, T2, T1)* t = args;                       \
    t->e3 = x;                                                           \
    return (Fn(T3, T2, T1, R)){                                          \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply3),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x) {       \
    FnEnv(name, T6, T5, T4, T3, T2, T1)* t = args;                       \
    t->e4 = x;                                                           \
    return (Fn(T2, T1, R)){                                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply2),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x) {           \
    FnEnv(name, T6, T5, T4, T3, T2, T1)* t = args;                       \
    t->e5 = x;                                                           \
    return (Fn(T1, R)){                                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T6, T5, T4, T3, T2, T1)* t = args;                       \
    t->e6 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name,                                               \
                     apply0)(FnEnv(name, T6, T5, T4, T3, T2, T1) * args)

// -----------------------------------------------------------------------
#define fn7(name, T7, T6, T5, T4, T3, T2, T1, R)                         \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T7, T6, T5, T4, T3, T2, T1);                       \
  /* ---- */                                                             \
  static Fn(T7, T6, T5, T4, T3, T2, T1, R) name(void);                   \
  static Fn(T6, T5, T4, T3, T2, T1, R)                                   \
      FUNC_NAME(name, apply7)(void* args, T7 x);                         \
  static Fn(T5, T4, T3, T2, T1, R)                                       \
      FUNC_NAME(name, apply6)(void* args, T6 x);                         \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x);                         \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x);    \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x);        \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x);            \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name, apply0)(                                      \
      FnEnv(name, T7, T6, T5, T4, T3, T2, T1) * args);                   \
  /* ---- */                                                             \
  static Fn(T7, T6, T5, T4, T3, T2, T1, R) name(void) {                  \
    return (Fn(T7, T6, T5, T4, T3, T2, T1, R)){                          \
        .args =                                                          \
            mem_malloc(sizeof(FnEnv(name, T7, T6, T5, T4, T3, T2, T1))), \
        .apply = FUNC_NAME(name, apply7),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T6, T5, T4, T3, T2, T1, R)                                   \
      FUNC_NAME(name, apply7)(void* args, T7 x) {                        \
    FnEnv(name, T7, T6, T5, T4, T3, T2, T1)* t = args;                   \
    t->e1 = x;                                                           \
    return (Fn(T6, T5, T4, T3, T2, T1, R)){                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply6),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T5, T4, T3, T2, T1, R)                                       \
      FUNC_NAME(name, apply6)(void* args, T6 x) {                        \
    FnEnv(name, T7, T6, T5, T4, T3, T2, T1)* t = args;                   \
    t->e2 = x;                                                           \
    return (Fn(T5, T4, T3, T2, T1, R)){                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply5),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x) {                        \
    FnEnv(name, T7, T6, T5, T4, T3, T2, T1)* t = args;                   \
    t->e3 = x;                                                           \
    return (Fn(T4, T3, T2, T1, R)){                                      \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply4),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x) {   \
    FnEnv(name, T7, T6, T5, T4, T3, T2, T1)* t = args;                   \
    t->e4 = x;                                                           \
    return (Fn(T3, T2, T1, R)){                                          \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply3),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x) {       \
    FnEnv(name, T7, T6, T5, T4, T3, T2, T1)* t = args;                   \
    t->e5 = x;                                                           \
    return (Fn(T2, T1, R)){                                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply2),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x) {           \
    FnEnv(name, T7, T6, T5, T4, T3, T2, T1)* t = args;                   \
    t->e6 = x;                                                           \
    return (Fn(T1, R)){                                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T7, T6, T5, T4, T3, T2, T1)* t = args;                   \
    t->e7 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name, apply0)(                                      \
      FnEnv(name, T7, T6, T5, T4, T3, T2, T1) * args)

// -----------------------------------------------------------------------
#define fn8(name, T8, T7, T6, T5, T4, T3, T2, T1, R)                     \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1);                   \
  /* ---- */                                                             \
  static Fn(T8, T7, T6, T5, T4, T3, T2, T1, R) name(void);               \
  static Fn(T7, T6, T5, T4, T3, T2, T1, R)                               \
      FUNC_NAME(name, apply8)(void* args, T8 x);                         \
  static Fn(T6, T5, T4, T3, T2, T1, R)                                   \
      FUNC_NAME(name, apply7)(void* args, T7 x);                         \
  static Fn(T5, T4, T3, T2, T1, R)                                       \
      FUNC_NAME(name, apply6)(void* args, T6 x);                         \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x);                         \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x);    \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x);        \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x);            \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name, apply0)(                                      \
      FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1) * args);               \
  /* ---- */                                                             \
  static Fn(T8, T7, T6, T5, T4, T3, T2, T1, R) name(void) {              \
    return (Fn(T8, T7, T6, T5, T4, T3, T2, T1, R)){                      \
        .args = mem_malloc(                                              \
            sizeof(FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1))),        \
        .apply = FUNC_NAME(name, apply8),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T7, T6, T5, T4, T3, T2, T1, R)                               \
      FUNC_NAME(name, apply8)(void* args, T8 x) {                        \
    FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;               \
    t->e1 = x;                                                           \
    return (Fn(T7, T6, T5, T4, T3, T2, T1, R)){                          \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply7),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T6, T5, T4, T3, T2, T1, R)                                   \
      FUNC_NAME(name, apply7)(void* args, T7 x) {                        \
    FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;               \
    t->e2 = x;                                                           \
    return (Fn(T6, T5, T4, T3, T2, T1, R)){                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply6),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T5, T4, T3, T2, T1, R)                                       \
      FUNC_NAME(name, apply6)(void* args, T6 x) {                        \
    FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;               \
    t->e3 = x;                                                           \
    return (Fn(T5, T4, T3, T2, T1, R)){                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply5),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x) {                        \
    FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;               \
    t->e4 = x;                                                           \
    return (Fn(T4, T3, T2, T1, R)){                                      \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply4),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x) {   \
    FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;               \
    t->e5 = x;                                                           \
    return (Fn(T3, T2, T1, R)){                                          \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply3),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x) {       \
    FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;               \
    t->e6 = x;                                                           \
    return (Fn(T2, T1, R)){                                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply2),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x) {           \
    FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;               \
    t->e7 = x;                                                           \
    return (Fn(T1, R)){                                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;               \
    t->e8 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name, apply0)(                                      \
      FnEnv(name, T8, T7, T6, T5, T4, T3, T2, T1) * args)

// -----------------------------------------------------------------------
#define fn9(name, T9, T8, T7, T6, T5, T4, T3, T2, T1, R)                 \
  C_API_BEGIN                                                            \
  /* ---- */                                                             \
  typedef_FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1);               \
  /* ---- */                                                             \
  static Fn(T9, T8, T7, T6, T5, T4, T3, T2, T1, R) name(void);           \
  static Fn(T8, T7, T6, T5, T4, T3, T2, T1, R)                           \
      FUNC_NAME(name, apply9)(void* args, T9 x);                         \
  static Fn(T7, T6, T5, T4, T3, T2, T1, R)                               \
      FUNC_NAME(name, apply8)(void* args, T8 x);                         \
  static Fn(T6, T5, T4, T3, T2, T1, R)                                   \
      FUNC_NAME(name, apply7)(void* args, T7 x);                         \
  static Fn(T5, T4, T3, T2, T1, R)                                       \
      FUNC_NAME(name, apply6)(void* args, T6 x);                         \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x);                         \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x);    \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x);        \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x);            \
  static R FUNC_NAME(name, apply1)(void* args, T1 x);                    \
  static R FUNC_NAME(name, apply0)(                                      \
      FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1) * args);           \
  /* ---- */                                                             \
  static Fn(T9, T8, T7, T6, T5, T4, T3, T2, T1, R) name(void) {          \
    return (Fn(T9, T8, T7, T6, T5, T4, T3, T2, T1, R)){                  \
        .args = mem_malloc(                                              \
            sizeof(FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1))),    \
        .apply = FUNC_NAME(name, apply9),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T8, T7, T6, T5, T4, T3, T2, T1, R)                           \
      FUNC_NAME(name, apply9)(void* args, T9 x) {                        \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e1 = x;                                                           \
    return (Fn(T8, T7, T6, T5, T4, T3, T2, T1, R)){                      \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply8),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T7, T6, T5, T4, T3, T2, T1, R)                               \
      FUNC_NAME(name, apply8)(void* args, T8 x) {                        \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e2 = x;                                                           \
    return (Fn(T7, T6, T5, T4, T3, T2, T1, R)){                          \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply7),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T6, T5, T4, T3, T2, T1, R)                                   \
      FUNC_NAME(name, apply7)(void* args, T7 x) {                        \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e3 = x;                                                           \
    return (Fn(T6, T5, T4, T3, T2, T1, R)){                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply6),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T5, T4, T3, T2, T1, R)                                       \
      FUNC_NAME(name, apply6)(void* args, T6 x) {                        \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e4 = x;                                                           \
    return (Fn(T5, T4, T3, T2, T1, R)){                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply5),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T4, T3, T2, T1, R)                                           \
      FUNC_NAME(name, apply5)(void* args, T5 x) {                        \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e5 = x;                                                           \
    return (Fn(T4, T3, T2, T1, R)){                                      \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply4),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T3, T2, T1, R) FUNC_NAME(name, apply4)(void* args, T4 x) {   \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e6 = x;                                                           \
    return (Fn(T3, T2, T1, R)){                                          \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply3),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T2, T1, R) FUNC_NAME(name, apply3)(void* args, T3 x) {       \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e7 = x;                                                           \
    return (Fn(T2, T1, R)){                                              \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply2),                                \
    };                                                                   \
  }                                                                      \
  static Fn(T1, R) FUNC_NAME(name, apply2)(void* args, T2 x) {           \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e8 = x;                                                           \
    return (Fn(T1, R)){                                                  \
        .args = args,                                                    \
        .apply = FUNC_NAME(name, apply1),                                \
    };                                                                   \
  }                                                                      \
  static R FUNC_NAME(name, apply1)(void* args, T1 x) {                   \
    FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1)* t = args;           \
    t->e9 = x;                                                           \
    return FUNC_NAME(name, apply0)(t);                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  /* ---- */                                                             \
  static R FUNC_NAME(name, apply0)(                                      \
      FnEnv(name, T9, T8, T7, T6, T5, T4, T3, T2, T1) * args)
