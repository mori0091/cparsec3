/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#define TYPE(T) TYPE_##T()
#define TYPE_int() trait(TExpr).Int()
#define TYPE_bool() trait(TExpr).Bool()
#define TYPE_unit() trait(TExpr).Unit()

typedef struct TExpr* TExpr;
decl_user_type(TExpr);

// -----------------------------------------------------------------------
/* Type variable */
typedef struct TVar {
  String ident;
} TVar;

/* Type constructor */
typedef struct TCon {
  String ident;
} TCon;

enum TExprId {
  /* type variable */
  TVAR,
  /* type constructor */
  TCON,
  /* type function application */
  TAPPLY,
};

struct TExpr {
  enum TExprId kind;
  union {
    /* for type variable */
    TVar tvar;
    /* for type constructor */
    TCon tcon;
    /* for type function application*/
    struct {
      TExpr lhs;
      TExpr rhs;
    };
  };
};

trait_Eq(TExpr);

// -----------------------------------------------------------------------
typedef struct TExprT {
  TExpr (*tvar)(TVar x);                 /* create type variable */
  TExpr (*tcon)(TCon x);                 /* create type constructor */
  TExpr (*tapply)(TExpr lhs, TExpr rhs); /* type function application */
  // ----
  TExpr (*Bool)(void); /* type constructor `bool` */
  TExpr (*Int)(void);  /* type constructor `int` */
  TExpr (*Unit)(void); /* type constructor `()` */
  TExpr (*Fn)(void);   /* type constructor `Fn` */
  // ----
  TExpr (*Undetermined)(void); /* type undeterined */
} TExprT;

TExprT Trait(TExpr);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

static bool FUNC_NAME(eq, Eq(TExpr))(TExpr a, TExpr b) {
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
  return FUNC_NAME(eq, Eq(TExpr))(a->lhs, b->lhs) &&
         FUNC_NAME(eq, Eq(TExpr))(a->rhs, b->rhs);
}
instance_Eq(TExpr, FUNC_NAME(eq, Eq(TExpr)));

static TExpr TExpr_New(void) {
  TExpr e = (TExpr)mem_malloc(sizeof(struct TExpr));
  return e;
}

static TExpr FUNC_NAME(tvar, TExpr)(TVar x) {
  TExpr e = TExpr_New();
  e->kind = TVAR;
  e->tvar = x;
  return e;
}

static TExpr FUNC_NAME(tcon, TExpr)(TCon x) {
  TExpr e = TExpr_New();
  e->kind = TCON;
  e->tcon = x;
  return e;
}

static TExpr FUNC_NAME(tapply, TExpr)(TExpr lhs, TExpr rhs) {
  TExpr e = TExpr_New();
  e->kind = TAPPLY;
  e->lhs = lhs;
  e->rhs = rhs;
  return e;
}

static TExpr FUNC_NAME(Bool, TExpr)(void) {
  static struct TExpr e = {.kind = TCON, .tcon = {"bool"}};
  return &e;
}

static TExpr FUNC_NAME(Int, TExpr)(void) {
  static struct TExpr e = {.kind = TCON, .tcon = {"int"}};
  return &e;
}

static TExpr FUNC_NAME(Unit, TExpr)(void) {
  static struct TExpr e = {.kind = TCON, .tcon = {"()"}};
  return &e;
}

static TExpr FUNC_NAME(Fn, TExpr)(void) {
  static struct TExpr e = {.kind = TCON, .tcon = {"Fn"}};
  return &e;
}

TExprT Trait(TExpr) {
  return (TExprT){
      .tvar = FUNC_NAME(tvar, TExpr),
      .tcon = FUNC_NAME(tcon, TExpr),
      .tapply = FUNC_NAME(tapply, TExpr),
      .Bool = FUNC_NAME(Bool, TExpr),
      .Int = FUNC_NAME(Int, TExpr),
      .Unit = FUNC_NAME(Unit, TExpr),
      .Fn = FUNC_NAME(Fn, TExpr),
  };
}

impl_user_type(TExpr);

show_user_type(TExpr)(CharBuff* b, TExpr x) {
  Show(TExpr) s = trait(Show(TExpr));
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
