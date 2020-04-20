/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Expr.h"

typedef struct MapEntry {
  String ident; ///< the name of the variable
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
    Maybe(Expr) (*lookup)(Context ctx, String ident);
    void (*put)(Context ctx, String ident, Expr e);
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

static Maybe(Expr) FUNC_NAME(lookup, Context)(Context ctx, String ident) {
  ListT(MapEntry) L = trait(List(MapEntry));
  while (ctx) {
    for (List(MapEntry) xs = ctx->map; !L.null(xs); xs = L.tail(xs)) {
      MapEntry x = L.head(xs);
      if (g_eq(x.ident, ident)) {
        return (Maybe(Expr)){.value = x.e};
      }
    }
    ctx = ctx->outer;
  }
  return (Maybe(Expr)){.none = true};
}

static void FUNC_NAME(put, Context)(Context ctx, String ident, Expr e) {
  MapEntry entry = {.ident = ident, .e = e};
  ctx->map = trait(List(MapEntry)).cons(entry, ctx->map);
}

ContextT Trait(Context) {
  return (ContextT){
      .create = FUNC_NAME(create, Context),
      .branch = FUNC_NAME(branch, Context),
      .nested = FUNC_NAME(nested, Context),
      .map.lookup = FUNC_NAME(lookup, Context),
      .map.put = FUNC_NAME(put, Context),
  };
}

#endif
