/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Expr.h"

typedef struct MapEntry {
  String ident; ///< the name of the variable
  Type type;    ///< type of the variable, or NULL if undetermined.
  Expr e;       ///< the expression bound to the variable
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
    MapEntry* (*lookup_local)(Context ctx, String ident);
    /**
     * lookup variable in the given context or in outer context
     * recursively.
     */
    MapEntry* (*lookup)(Context ctx, String ident);
    /**
     * add variable to the given context.
     */
    void (*put)(Context ctx, String ident, Type type, Expr e);
  } map;
} ContextT;

ContextT Trait(Context);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

#include <cparsec3/base/base_generics.h>

impl_List(MapEntry);

static Context FUNC_NAME(create, Context)(void) {
  Context ctx = mem_malloc(sizeof(struct Context));
  ctx->outer = NULL;
  ctx->map = trait(List(MapEntry)).empty;
  return ctx;
}

static Context FUNC_NAME(branch, Context)(Context c) {
  Context ctx = mem_malloc(sizeof(struct Context));
  ctx->outer = c->outer;
  ctx->map = c->map;
  return ctx;
}

static Context FUNC_NAME(nested, Context)(Context c) {
  Context ctx = mem_malloc(sizeof(struct Context));
  ctx->outer = c;
  ctx->map = trait(List(MapEntry)).empty;
  return ctx;
}

static MapEntry* FUNC_NAME(lookup_local, Context)(Context ctx,
                                                  String ident) {
  ListT(MapEntry) L = trait(List(MapEntry));
  for (List(MapEntry) xs = ctx->map; !L.null(xs); xs = L.tail(xs)) {
    MapEntry* x = &xs->head;
    if (g_eq(x->ident, ident)) {
      return x;
    }
  }
  return NULL;
}

static MapEntry* FUNC_NAME(lookup, Context)(Context ctx, String ident) {
  while (ctx) {
    MapEntry* x = FUNC_NAME(lookup_local, Context)(ctx, ident);
    if (x) {
      return x;
    }
    ctx = ctx->outer;
  }
  return NULL;
}

static void FUNC_NAME(put, Context)(Context ctx, String ident, Type type,
                                    Expr e) {
  MapEntry entry = {.ident = ident, .type = type, .e = e};
  ctx->map = trait(List(MapEntry)).cons(entry, ctx->map);
}

ContextT Trait(Context) {
  return (ContextT){
      .create = FUNC_NAME(create, Context),
      .branch = FUNC_NAME(branch, Context),
      .nested = FUNC_NAME(nested, Context),
      .map.lookup_local = FUNC_NAME(lookup_local, Context),
      .map.lookup = FUNC_NAME(lookup, Context),
      .map.put = FUNC_NAME(put, Context),
  };
}

#endif
