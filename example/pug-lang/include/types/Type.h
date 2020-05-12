/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#include "Kind.h"

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
  Kind kind;
} TVar;

/* Type constructor */
typedef struct TCon {
  String ident;
} TCon;

/* Universal quantified type variable (e.g. ∀a) */
typedef struct TGen {
  int n;
} TGen;

enum TypeId {
  /* type variable */
  TVAR,
  /* type constructor */
  TCON,
  /* type application */
  TAPPLY,
  /* universal quantified type variable (e.g. ∀a) */
  TGEN,
};

struct Type {
  enum TypeId id;
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
    TGen tgen;
  };
};

trait_Eq(TVar);
trait_Eq(TCon);
trait_Eq(TGen);
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
  Type (*tgen)(TGen x);
  // ---- helper to create builtin type constructors
  Type (*tcon_bool)(void);   /* type constructor `bool` */
  Type (*tcon_int)(void);    /* type constructor `int` */
  Type (*tcon_unit)(void);   /* type constructor `()` */
  Type (*tcon_Fn)(void);     /* type constructor `Fn` */
  Type (*tcon_List)(void);   /* type constructor `[,]` */
  Type (*tcon_Tuple2)(void); /* type constructor `(,)` */
  // ---- helper to create type of a function, list, and 2-tuple
  Type (*func)(Type arg, Type ret);
  Type (*list)(Type arg);
  Type (*pair)(Type a, Type b);
} TypeT;

TypeT Trait(Type);
