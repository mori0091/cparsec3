/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base_generics.h>

#include "Type.h"
#include "TypeAssumption.h"
#include "TypeScheme.h"
#include "TypeSubst.h"

typedef struct TIState {
  /** current type substitution */
  TypeSubst subst;
  /** next index for creating universal quantified type variable */
  int i;
} TIState;

typedef struct TypeError {
  String msg;
} TypeError;

#define TypeInfer(T) TYPE_NAME(TypeInfer, T)
#define TIResult(T) TYPE_NAME(TIResult, T)
#define TIRunner(T) TYPE_NAME(TIRunner, T)

// -----------------------------------------------------------------------
#define TIOk(...) Fn(TIOkArgs(__VA_ARGS__))
#define typedef_TIOk(...) typedef_Fn(TIOkArgs(__VA_ARGS__))
#define TIOkArgs(T)                                                      \
  T,              /* return type of TypeInfer */                         \
      TIState,    /* next input state */                                 \
      TIResult(T) /* return type of the continuation */

// -----------------------------------------------------------------------
#define TIErr(...) Fn(TIErrArgs(__VA_ARGS__))
#define typedef_TIErr(...) typedef_Fn(TIErrArgs(__VA_ARGS__))
#define TIErrArgs(T)                                                     \
  TypeError,      /* an error occurred */                                \
      TIState,    /* next input state */                                 \
      TIResult(T) /* return type of the continuation */

// -----------------------------------------------------------------------
#define UnTypeInfer(...) Fn(UnTypeInferArgs(__VA_ARGS__))
#define typedef_UnTypeInfer(...)                                         \
  typedef_Fn_r(UnTypeInferArgs(__VA_ARGS__))
#define UnTypeInferArgs(T)                                               \
  TIState,        /* input state */                                      \
      TIOk(T),    /* continuation called when 'ok' */                    \
      TIErr(T),   /* continuation called when 'error' */                 \
      TIResult(T) /* return type of the continuation */

// -----------------------------------------------------------------------
#define trait_TIRunner(T)                                                \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct TIResult(T) TIResult(T);                                \
  struct TIResult(T) {                                                   \
    TIState state;                                                       \
    bool success;                                                        \
    union {                                                              \
      T ok;                                                              \
      TypeError err;                                                     \
    };                                                                   \
  };                                                                     \
                                                                         \
  typedef_Fn(TIState, TIResult(T));                                      \
  typedef_TIOk(T);                                                       \
  typedef_TIErr(T);                                                      \
  typedef_UnTypeInfer(T);                                                \
                                                                         \
  typedef struct TypeInfer(T) TypeInfer(T);                              \
  struct TypeInfer(T) {                                                  \
    UnTypeInfer(T) unTypeInfer;                                          \
  };                                                                     \
                                                                         \
  typedef struct TIRunner(T) TIRunner(T);                                \
  struct TIRunner(T) {                                                   \
    TIResult(T) (*pRunTypeInferP)(TypeInfer(T) ti, TIState state);       \
    TIResult(T) (*pRunTypeInfer)(TypeInfer(T) ti);                       \
  };                                                                     \
                                                                         \
  TIRunner(T) Trait(TIRunner(T));                                        \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_TIRunner(T)                                                 \
  C_API_BEGIN                                                            \
                                                                         \
  impl_pRunTypeInferP(T);                                                \
  impl_pRunTypeInfer(T);                                                 \
                                                                         \
  TIRunner(T) Trait(TIRunner(T)) {                                       \
    return (TIRunner(T)){                                                \
        .pRunTypeInferP = FUNC_NAME(pRunTypeInferP, T),                  \
        .pRunTypeInfer = FUNC_NAME(pRunTypeInfer, T),                    \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_pRunTypeInferP(T)                                           \
                                                                         \
  fn(FUNC_NAME(tiOk, T), TIOkArgs(T)) {                                  \
    g_bind((a, state), *args);                                           \
    return (TIResult(T)){                                                \
        .state = state,                                                  \
        .success = true,                                                 \
        .ok = a,                                                         \
    };                                                                   \
  }                                                                      \
                                                                         \
  fn(FUNC_NAME(tiErr, T), TIErrArgs(T)) {                                \
    g_bind((e, state), *args);                                           \
    return (TIResult(T)){                                                \
        .state = state,                                                  \
        .success = false,                                                \
        .err = e,                                                        \
    };                                                                   \
  }                                                                      \
                                                                         \
  static TIResult(T)                                                     \
      FUNC_NAME(pRunTypeInferP, T)(TypeInfer(T) ti, TIState s) {         \
    __auto_type ok = FUNC_NAME(tiOk, T)();                               \
    __auto_type err = FUNC_NAME(tiErr, T)();                             \
    return fn_apply(ti.unTypeInfer, s, ok, err);                         \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_pRunTypeInfer(T)                                            \
                                                                         \
  static TIResult(T) FUNC_NAME(pRunTypeInfer, T)(TypeInfer(T) ti) {      \
    TIState s = {.subst = NULL, .i = 0};                                 \
    return FUNC_NAME(pRunTypeInferP, T)(ti, s);                          \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define TYPE_INFER_RETURN_TYPES() None, Type, TypeSubst, TypeScheme

trait_TIRunner(None);
trait_TIRunner(Type);
trait_TIRunner(TypeSubst);
trait_TIRunner(TypeScheme);
