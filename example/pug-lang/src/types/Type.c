/* -*- coding: utf-8-unix -*- */

#include "types/Type.h"

// -------------------------------------
// trait Eq(TVar)

static bool FUNC_NAME(eq, Eq(TVar))(TVar a, TVar b) {
  return (a.ident == b.ident) || trait(Eq(String)).eq(a.ident, b.ident);
}
instance_Eq(TVar, FUNC_NAME(eq, Eq(TVar)));

// -------------------------------------
// trait Eq(TCon)

static bool FUNC_NAME(eq, Eq(TCon))(TCon a, TCon b) {
  return (a.ident == b.ident) || trait(Eq(String)).eq(a.ident, b.ident);
}
instance_Eq(TCon, FUNC_NAME(eq, Eq(TCon)));

// -------------------------------------
// trait Eq(Type)

static bool FUNC_NAME(eq, Eq(Type))(Type a, Type b) {
  if (a == b) {
    return true;
  }
  if (!a || !b) {
    return false; // type of either `a` or `b` is undetermined
  }
  if (a->kind != b->kind) {
    return false;
  }
  if (a->kind == TVAR) {
    return trait(Eq(TVar)).eq(a->tvar, b->tvar);
  }
  if (a->kind == TCON) {
    return trait(Eq(TCon)).eq(a->tcon, b->tcon);
  }
  return FUNC_NAME(eq, Eq(Type))(a->lhs, b->lhs) &&
         FUNC_NAME(eq, Eq(Type))(a->rhs, b->rhs);
}
instance_Eq(Type, FUNC_NAME(eq, Eq(Type)));

// -------------------------------------
// trait Type

static Type Type_New(void) {
  Type e = (Type)mem_malloc(sizeof(struct Type));
  return e;
}

static Type FUNC_NAME(tvar, Type)(TVar x) {
  Type e = Type_New();
  e->kind = TVAR;
  e->tvar = x;
  return e;
}

static Type FUNC_NAME(tcon, Type)(TCon x) {
  Type e = Type_New();
  e->kind = TCON;
  e->tcon = x;
  return e;
}

static Type FUNC_NAME(tapply, Type)(Type lhs, Type rhs) {
  Type e = Type_New();
  e->kind = TAPPLY;
  e->lhs = lhs;
  e->rhs = rhs;
  return e;
}

static Type FUNC_NAME(tcon_bool, Type)(void) {
  static struct Type e = {.kind = TCON, .tcon = {"bool"}};
  return &e;
}

static Type FUNC_NAME(tcon_int, Type)(void) {
  static struct Type e = {.kind = TCON, .tcon = {"int"}};
  return &e;
}

static Type FUNC_NAME(tcon_unit, Type)(void) {
  static struct Type e = {.kind = TCON, .tcon = {"()"}};
  return &e;
}

static Type FUNC_NAME(tcon_Fn, Type)(void) {
  static struct Type e = {.kind = TCON, .tcon = {"Fn"}};
  return &e;
}

static Type FUNC_NAME(funcType, Type)(Type arg, Type ret) {
  TypeT t = trait(Type);
  return t.tapply(t.tapply(t.tcon_Fn(), arg), ret);
}

TypeT Trait(Type) {
  return (TypeT){
      .tvar = FUNC_NAME(tvar, Type),
      .tcon = FUNC_NAME(tcon, Type),
      .tapply = FUNC_NAME(tapply, Type),
      .tcon_bool = FUNC_NAME(tcon_bool, Type),
      .tcon_int = FUNC_NAME(tcon_int, Type),
      .tcon_unit = FUNC_NAME(tcon_unit, Type),
      .tcon_Fn = FUNC_NAME(tcon_Fn, Type),
      .funcType = FUNC_NAME(funcType, Type),
  };
}

impl_user_type(Type);

show_user_type(Type)(CharBuff* b, Type x) {
  if (!x) {
    mem_printf(b, "<unknown>");
    return;
  }
  Show(Type) s = trait(Show(Type));
  switch (x->kind) {
  case TVAR:
    mem_printf(b, "(TVar %s)", x->tvar.ident);
    break;
  case TCON:
    mem_printf(b, "%s", x->tcon.ident);
    break;
  case TAPPLY:
    mem_printf(b, "(TApply ");
    s.toString(b, x->lhs);
    mem_printf(b, " ");
    s.toString(b, x->rhs);
    mem_printf(b, ")");
    break;
  default:
    assert(0 && "Illegal Type");
    break;
  }
}
