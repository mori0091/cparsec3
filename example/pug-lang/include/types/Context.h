/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Expr.h"

typedef struct MapEntry {
  Id ident;  ///< the name of the variable
  Type type; ///< type of the variable, or NULL if undetermined.
  Expr e;    ///< the expression bound to the variable
} MapEntry;

trait_List(MapEntry);

typedef struct Context* Context;
struct Context {
  Context outer;      ///< enclosing context, or NULL
  List(MapEntry) map; ///< list of variables accessible in this scope
};

typedef struct ContextT {
  Context (*create)(void);        ///< creates top-level context
  Context (*branch)(Context ctx); ///< creates branced context
  Context (*nested)(Context ctx); ///< creates nested (inner) context
  struct {
    /**
     * lookup variable in the given context.
     */
    MapEntry* (*lookup_local)(Context ctx, Id ident);
    /**
     * lookup variable in the given context or in outer context
     * recursively.
     */
    MapEntry* (*lookup)(Context ctx, Id ident);
    /**
     * add variable to the given context.
     */
    void (*put)(Context ctx, Id ident, Type type, Expr e);
  } map;
} ContextT;

ContextT Trait(Context);
