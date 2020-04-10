/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#define Address int

typedef struct MapEntry {
  String ident;
  Address addr;
} MapEntry;

trait_Mem(MapEntry);
trait_List(MapEntry);

typedef struct Context {
  Address frame_pointer;
  Array(int) stack;
  List(MapEntry) map; /* String => Address */
} Context;

typedef struct ContextT {
  Context* (*create)(size_t stack_length);
  struct {
    Maybe(Address) (*push)(Context* ctx, int x);
    Maybe(int) (*pop)(Context* ctx);
    Maybe(int) (*load)(Context* ctx, Address addr);
    Maybe(int) (*store)(Context* ctx, Address addr, int x);
  } stack;
  struct {
    Maybe(Address) (*lookup)(Context* ctx, String ident);
    Maybe(Address) (*put)(Context* ctx, String ident);
  } map;
} ContextT;

ContextT Trait(Context);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

impl_Mem(MapEntry);
impl_List(MapEntry);

static Context* FUNC_NAME(create, Context)(size_t stack_length) {
  Context* ctx = mem_malloc(sizeof(struct Context));
  ctx->frame_pointer = 0;
  ctx->stack = trait(Array(int)).create(stack_length);
  ctx->map = trait(List(MapEntry)).empty;
  return ctx;
}

static Maybe(Address) FUNC_NAME(push, Context)(Context* ctx, int x) {
  if ((Address)ctx->stack.length <= ctx->frame_pointer) {
    return (Maybe(Address)){.none = true};
  }
  Address a = ctx->frame_pointer++;
  ctx->stack.data[a] = x;
  return (Maybe(Address)){.value = a};
}

static Maybe(int) FUNC_NAME(pop, Context)(Context* ctx) {
  if (ctx->frame_pointer <= 0) {
    return (Maybe(int)){.none = true};
  }
  Address a = --ctx->frame_pointer;
  int x = ctx->stack.data[a];
  return (Maybe(int)){.value = x};
}

static Maybe(int) FUNC_NAME(load, Context)(Context* ctx, Address addr) {
  if (addr < 0 || ctx->frame_pointer <= addr) {
    return (Maybe(int)){.none = true};
  }
  int x = ctx->stack.data[addr];
  return (Maybe(int)){.value = x};
}

static Maybe(int)
    FUNC_NAME(store, Context)(Context* ctx, Address addr, int x) {
  if (addr < 0 || ctx->frame_pointer <= addr) {
    return (Maybe(int)){.none = true};
  }
  ctx->stack.data[addr] = x;
  return (Maybe(int)){.value = x};
}

static Maybe(Address)
    FUNC_NAME(lookup, Context)(Context* ctx, String ident) {
  ListT(MapEntry) L = trait(List(MapEntry));
  for (List(MapEntry) xs = ctx->map; !L.null(xs); xs = L.tail(xs)) {
    MapEntry x = L.head(xs);
    if (g_eq(x.ident, ident)) {
      return (Maybe(Address)){.value = x.addr};
    }
  }
  return (Maybe(Address)){.none = true};
}

static Maybe(Address)
    FUNC_NAME(put, Context)(Context* ctx, String ident) {
  Maybe(Address) m = FUNC_NAME(push, Context)(ctx, 0);
  if (!m.none) {
    MapEntry e = {.ident = ident, .addr = m.value};
    ctx->map = trait(List(MapEntry)).cons(e, ctx->map);
  }
  return m;
}

ContextT Trait(Context) {
  return (ContextT){
      .create = FUNC_NAME(create, Context),
      .stack.push = FUNC_NAME(push, Context),
      .stack.pop = FUNC_NAME(pop, Context),
      .stack.load = FUNC_NAME(load, Context),
      .stack.store = FUNC_NAME(store, Context),
      .map.lookup = FUNC_NAME(lookup, Context),
      .map.put = FUNC_NAME(put, Context),
  };
}

#endif
