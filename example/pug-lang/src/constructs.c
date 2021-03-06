/* -*- coding: utf-8-unix -*- */

#include "constructs.h"

Type makeSimpleType(Id name, Array(Type) argTypes) {
  ArrayT(Type) A = trait(Array(Type));
  TypeT T = trait(Type);
  KindT K = trait(Kind);
  Kind k = K.Star();
  for (size_t n = A.length(argTypes); n; n--) {
    k = K.Kfun(K.Star(), k);
  }
  Type lhs = T.TCon((Tycon){name, k});
  for (Type* t = A.begin(argTypes); t != A.end(argTypes); t++) {
    lhs = T.TAp(lhs, *t);
  }
  // A.free(&argTypes);
  return lhs;
}

Type makeTypeSig(Array(Type) argTypes, Type retType) {
  TypeT T = trait(Type);
  ArrayT(Type) A = trait(Array(Type));
  for (Type* t = A.end(argTypes); t != A.begin(argTypes);) {
    retType = T.func(*(--t), retType);
  }
  return retType;
}

List(Expr) makeVars(size_t n) {
  ExprT E = trait(Expr);
  ListT(Expr) L = trait(List(Expr));
  List(Expr) xs = NULL;
  while (n--) {
    CharBuff b = {0};
    mem_printf(&b, "a%zu", n);
    xs = L.cons(E.var(b.data), xs);
  }
  return xs;
}

Expr makeLambda(List(Expr) vars, Expr body) {
  if (!vars) {
    return body;
  }
  ExprT E = trait(Expr);
  return E.lambda(vars->head, makeLambda(vars->tail, body));
}

Expr defineConstructor(Id name, Array(Type) argTypes, Type retType) {
  ExprT E = trait(Expr);
  Expr x = E.var(name);
  Type typeSig = makeTypeSig(argTypes, retType);
  List(Expr) vars = makeVars(argTypes.length);
  Expr body = E.con(name, vars);
  Expr lam = makeLambda(vars, body);
  return E.seq(E.declvar(x, E.type(typeSig)), E.let(x, lam));
}
