/* -*- coding: utf-8-unix -*- */

#include "typesystem/type_checker.h"

#define CPARSEC_CONFIG_ACTION_IMPLEMENT /* generate Action code */
#include "types/Infer.h"
#include "types/TypeInfer.h"

// -----------------------------------------------------------------------
static inline TIResult(Tup(List(Pred), Type))
  testInferP(ClassEnv ce, List(Assump) as, Expr e) {
  return runTI(tiProgram(ce, as, e), ((TIState){0}));
}

static inline TIResult(Tup(List(Pred), Type)) testInfer(Expr e) {
  ClassEnv ce = initialEnv();
  return testInferP(ce, NULL, e);
}

// -----------------------------------------------------------------------
Result(Type, String) typeOf(Expr e) {
  TIResult(Tup(List(Pred), Type)) r = testInfer(e);
  if (!r.success) {
    return (Result(Type, String)){
        .err = r.err.msg,
    };
  }
  return (Result(Type, String)){
      .success = true,
      .ok = r.ok.t,
  };
}
