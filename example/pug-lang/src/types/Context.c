/* -*- coding: utf-8-unix -*- */

#include "types/Context.h"

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

static void FUNC_NAME(put, Context)(Context ctx, String ident, Expr e) {
  MapEntry entry = {.ident = ident, .e = e};
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
