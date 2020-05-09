/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#define TYPE(T) TYPE_##T()
#define TYPE_int() trait(Type).tcon_int()
#define TYPE_bool() trait(Type).tcon_bool()
#define TYPE_unit() trait(Type).tcon_unit()

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

/* Universal quantified type variable (e.g. ∀a) */
typedef struct TAny {
  int n;
} TAny;

enum TypeId {
  /* type variable */
  TVAR,
  /* type constructor */
  TCON,
  /* type application */
  TAPPLY,
  /* universal quantified type variable (e.g. ∀a) */
  TANY,
};

struct Type {
  enum TypeId kind;
  union {
    /* for type variable */
    TVar tvar;
    /* for type constructor */
    TCon tcon;
    /* for type application*/
    struct {
      Type lhs;
      Type rhs;
    };
    /* for universal quantified type variable */
    TAny any;
  };
};

trait_Eq(TVar);
trait_Eq(TCon);
trait_Eq(TAny);
trait_Eq(Type);

// -----------------------------------------------------------------------
typedef struct TypeT {
  /** create type variable */
  Type (*tvar)(TVar x);
  /** create type constructor */
  Type (*tcon)(TCon x);
  /** type application */
  Type (*tapply)(Type lhs, Type rhs);
  /** create universal quantified type variable */
  Type (*any)(TAny x);
  // ---- helper to create builtin type constructors
  Type (*tcon_bool)(void); /* type constructor `bool` */
  Type (*tcon_int)(void);  /* type constructor `int` */
  Type (*tcon_unit)(void); /* type constructor `()` */
  Type (*tcon_Fn)(void);   /* type constructor `Fn` */
  // ---- helper to create type of a function
  Type (*funcType)(Type arg, Type ret);
} TypeT;

TypeT Trait(Type);
