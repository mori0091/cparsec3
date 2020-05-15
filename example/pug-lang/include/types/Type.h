/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#include "Kind.h"

#define TYPE(T) TYPE_##T()
#define TYPE_int() trait(Type).tcon_int()
#define TYPE_bool() trait(Type).tcon_bool()
#define TYPE_unit() trait(Type).tcon_unit()

C_API_BEGIN

typedef struct Type* Type;
decl_user_type(Type);

// -----------------------------------------------------------------------
/* Type variable */
typedef struct Tyvar {
  String ident;
  Kind kind;
} Tyvar;

/* Type constructor */
typedef struct Tycon {
  String ident;
  Kind kind;
} Tycon;

/* Universal quantified type variable (e.g. ∀a) */
typedef struct Tygen {
  int n;
} Tygen;

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
    Tyvar tvar;
    /* for type constructor */
    Tycon tcon;
    /* for type application*/
    struct {
      Type lhs;
      Type rhs;
    };
    /* for universal quantified type variable */
    Tygen tgen;
  };
};

trait_Eq(Tyvar);
trait_Eq(Tycon);
trait_Eq(Tygen);
trait_Eq(Type);

trait_List(Type);
trait_Eq(List(Type));

// -----------------------------------------------------------------------
typedef struct TypeT {
  /** create type variable */
  Type (*TVar)(Tyvar v);
  /** create type constructor */
  Type (*TCon)(Tycon c);
  /** type application */
  Type (*TAp)(Type lhs, Type rhs);
  /** create universal quantified type variable */
  Type (*TGen)(int n);
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

C_API_END

// -----------------------------------------------------------------------
#define HasKind(T) TYPE_NAME(HasKind, T)

#define trait_HasKind(T)                                                 \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct HasKind(T) {                                            \
    Kind (*kind)(T t);                                                   \
  }                                                                      \
  HasKind(T);                                                            \
                                                                         \
  HasKind(T) Trait(HasKind(T));                                          \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

trait_HasKind(Tyvar);
trait_HasKind(Tycon);
trait_HasKind(Type);
