/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#define Address int

typedef struct MapEntry {
  String ident;
  Expr e;
} MapEntry;

trait_List(MapEntry);

typedef struct Context {
  List(MapEntry) map; /* String => Address */
} Context;

typedef struct ContextT {
  Context* (*create)(void);
  struct {
    Maybe(Expr) (*lookup)(Context* ctx, String ident);
    void (*put)(Context* ctx, String ident, Expr e);
  } map;
} ContextT;

ContextT Trait(Context);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

impl_List(MapEntry);

static Context* FUNC_NAME(create, Context)(void) {
  Context* ctx = mem_malloc(sizeof(struct Context));
  ctx->map = trait(List(MapEntry)).empty;
  return ctx;
}

static Maybe(Expr)
    FUNC_NAME(lookup, Context)(Context* ctx, String ident) {
  ListT(MapEntry) L = trait(List(MapEntry));
  for (List(MapEntry) xs = ctx->map; !L.null(xs); xs = L.tail(xs)) {
    MapEntry x = L.head(xs);
    if (g_eq(x.ident, ident)) {
      return (Maybe(Expr)){.value = x.e};
    }
  }
  return (Maybe(Expr)){.none = true};
}

static void FUNC_NAME(put, Context)(Context* ctx, String ident, Expr e) {
  MapEntry entry = {.ident = ident, .e = e};
  ctx->map = trait(List(MapEntry)).cons(entry, ctx->map);
}

ContextT Trait(Context) {
  return (ContextT){
      .create = FUNC_NAME(create, Context),
      .map.lookup = FUNC_NAME(lookup, Context),
      .map.put = FUNC_NAME(put, Context),
  };
}

#endif
