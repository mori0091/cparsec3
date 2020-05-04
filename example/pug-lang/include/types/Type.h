/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#define TYPE(T) TYPE_##T()
#define TYPE_int() trait(Type).Int()
#define TYPE_bool() trait(Type).Bool()
#define TYPE_unit() trait(Type).Unit()

typedef struct Type* Type;
decl_user_type(Type);

// -----------------------------------------------------------------------
/* Type variable */
typedef struct TVar {
  String ident;
} TVar;

/* Type constructor */
typedef struct TCon {
  String ident;
} TCon;

enum TypeId {
  /* type variable */
  TVAR,
  /* type constructor */
  TCON,
  /* type function application */
  TAPPLY,
};

struct Type {
  enum TypeId kind;
  union {
    /* for type variable */
    TVar tvar;
    /* for type constructor */
    TCon tcon;
    /* for type function application*/
    struct {
      Type lhs;
      Type rhs;
    };
  };
};

trait_Eq(Type);

// -----------------------------------------------------------------------
typedef struct TypeT {
  Type (*tvar)(TVar x);               /* create type variable */
  Type (*tcon)(TCon x);               /* create type constructor */
  Type (*tapply)(Type lhs, Type rhs); /* type function application */
  // ----
  Type (*Bool)(void); /* type constructor `bool` */
  Type (*Int)(void);  /* type constructor `int` */
  Type (*Unit)(void); /* type constructor `()` */
  Type (*Fn)(void);   /* type constructor `Fn` */
} TypeT;

TypeT Trait(Type);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

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
    return (a->tvar.ident == b->tvar.ident) ||
           trait(Eq(String)).eq(a->tvar.ident, b->tvar.ident);
  }
  if (a->kind == TCON) {
    return (a->tcon.ident == b->tcon.ident) ||
           trait(Eq(String)).eq(a->tcon.ident, b->tcon.ident);
  }
  return FUNC_NAME(eq, Eq(Type))(a->lhs, b->lhs) &&
         FUNC_NAME(eq, Eq(Type))(a->rhs, b->rhs);
}
instance_Eq(Type, FUNC_NAME(eq, Eq(Type)));

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

static Type FUNC_NAME(Bool, Type)(void) {
  static struct Type e = {.kind = TCON, .tcon = {"bool"}};
  return &e;
}

static Type FUNC_NAME(Int, Type)(void) {
  static struct Type e = {.kind = TCON, .tcon = {"int"}};
  return &e;
}

static Type FUNC_NAME(Unit, Type)(void) {
  static struct Type e = {.kind = TCON, .tcon = {"()"}};
  return &e;
}

static Type FUNC_NAME(Fn, Type)(void) {
  static struct Type e = {.kind = TCON, .tcon = {"Fn"}};
  return &e;
}

TypeT Trait(Type) {
  return (TypeT){
      .tvar = FUNC_NAME(tvar, Type),
      .tcon = FUNC_NAME(tcon, Type),
      .tapply = FUNC_NAME(tapply, Type),
      .Bool = FUNC_NAME(Bool, Type),
      .Int = FUNC_NAME(Int, Type),
      .Unit = FUNC_NAME(Unit, Type),
      .Fn = FUNC_NAME(Fn, Type),
  };
}

impl_user_type(Type);

show_user_type(Type)(CharBuff* b, Type x) {
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

#endif
