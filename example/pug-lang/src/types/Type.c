/* -*- coding: utf-8-unix -*- */

#include "types/Type.h"

// -------------------------------------
// trait Eq(Tyvar)

static bool FUNC_NAME(eq, Eq(Tyvar))(Tyvar a, Tyvar b) {
  return ((a.ident == b.ident) ||
          trait(Eq(String)).eq(a.ident, b.ident)) &&
         trait(Eq(Kind)).eq(a.kind, b.kind);
}
instance_Eq(Tyvar, FUNC_NAME(eq, Eq(Tyvar)));

// -------------------------------------
// trait Eq(Tycon)

static bool FUNC_NAME(eq, Eq(Tycon))(Tycon a, Tycon b) {
  return ((a.ident == b.ident) ||
          trait(Eq(String)).eq(a.ident, b.ident)) &&
         trait(Eq(Kind)).eq(a.kind, b.kind);
}
instance_Eq(Tycon, FUNC_NAME(eq, Eq(Tycon)));

// -------------------------------------
// trait Eq(Tygen)

static bool FUNC_NAME(eq, Eq(Tygen))(Tygen a, Tygen b) {
  return (a.n == b.n);
}
instance_Eq(Tygen, FUNC_NAME(eq, Eq(Tygen)));

// -------------------------------------
// trait Eq(Type)

static bool FUNC_NAME(eq, Eq(Type))(Type a, Type b) {
  if (a == b) {
    return true;
  }
  if (!a || !b) {
    return false; // type of either `a` or `b` is undetermined
  }
  if (a->id != b->id) {
    return false;
  }
  if (a->id == TVAR) {
    return trait(Eq(Tyvar)).eq(a->tvar, b->tvar);
  }
  if (a->id == TCON) {
    return trait(Eq(Tycon)).eq(a->tcon, b->tcon);
  }
  if (a->id == TGEN) {
    return trait(Eq(Tygen)).eq(a->tgen, b->tgen);
  }
  return FUNC_NAME(eq, Eq(Type))(a->lhs, b->lhs) &&
         FUNC_NAME(eq, Eq(Type))(a->rhs, b->rhs);
}
instance_Eq(Type, FUNC_NAME(eq, Eq(Type)));

// -------------------------------------
// trait List(Type)
impl_List(Type);

// -------------------------------------
// trait Eq(List(Type))
impl_Eq_List(Type);

// -------------------------------------
// trait Type

static Type Type_New(void) {
  Type e = (Type)mem_malloc(sizeof(struct Type));
  return e;
}

static Type FUNC_NAME(TVar, Type)(Tyvar v) {
  assert(v.kind && "Null pointer");
  Type e = Type_New();
  e->id = TVAR;
  e->tvar = v;
  return e;
}

static Type FUNC_NAME(TCon, Type)(Tycon c) {
  assert(c.kind && "Null pointer");
  Type e = Type_New();
  e->id = TCON;
  e->tcon = c;
  return e;
}

static Type FUNC_NAME(TAp, Type)(Type lhs, Type rhs) {
  Type e = Type_New();
  e->id = TAPPLY;
  e->lhs = lhs;
  e->rhs = rhs;
  return e;
}

static Type FUNC_NAME(TGen, Type)(int n) {
  Type e = Type_New();
  e->id = TGEN;
  e->tgen.n = n;
  return e;
}

static Type FUNC_NAME(tcon_bool, Type)(void) {
  static struct Type e = {
      .id = TCON,
      .tcon.ident = "bool",
  };
  e.tcon.kind = trait(Kind).Star();
  return &e;
}

static Type FUNC_NAME(tcon_int, Type)(void) {
  static struct Type e = {
      .id = TCON,
      .tcon.ident = "int",
  };
  e.tcon.kind = trait(Kind).Star();
  return &e;
}

static Type FUNC_NAME(tcon_unit, Type)(void) {
  static struct Type e = {
      .id = TCON,
      .tcon.ident = "()",
  };
  e.tcon.kind = trait(Kind).Star();
  return &e;
}

static Type FUNC_NAME(tcon_Fn, Type)(void) {
  static struct Type e = {
      .id = TCON,
      .tcon.ident = "Fn",
  };
  e.tcon.kind = trait(Kind).Star_Star_Star();
  return &e;
}

static Type FUNC_NAME(tcon_List, Type)(void) {
  static struct Type e = {
      .id = TCON,
      .tcon.ident = "[,]",
  };
  e.tcon.kind = trait(Kind).Star_Star();
  return &e;
}

static Type FUNC_NAME(tcon_Tuple2, Type)(void) {
  static struct Type e = {
      .id = TCON,
      .tcon.ident = "(,)",
  };
  e.tcon.kind = trait(Kind).Star_Star_Star();
  return &e;
}

static Type FUNC_NAME(tcon_String, Type)(void) {
  static struct Type e = {
    .id = TCON,
    .tcon.ident = "String",
  };
  e.tcon.kind = trait(Kind).Star();
  return &e;
}

static Type FUNC_NAME(func, Type)(Type arg, Type ret) {
  TypeT t = trait(Type);
  return t.TAp(t.TAp(t.tcon_Fn(), arg), ret);
}

static Type FUNC_NAME(list, Type)(Type arg) {
  TypeT t = trait(Type);
  return t.TAp(t.tcon_List(), arg);
}

static Type FUNC_NAME(pair, Type)(Type a, Type b) {
  TypeT t = trait(Type);
  return t.TAp(t.TAp(t.tcon_Tuple2(), a), b);
}

TypeT Trait(Type) {
  return (TypeT){
      .TVar = FUNC_NAME(TVar, Type),
      .TCon = FUNC_NAME(TCon, Type),
      .TAp = FUNC_NAME(TAp, Type),
      .TGen = FUNC_NAME(TGen, Type),
      .tcon_bool = FUNC_NAME(tcon_bool, Type),
      .tcon_int = FUNC_NAME(tcon_int, Type),
      .tcon_unit = FUNC_NAME(tcon_unit, Type),
      .tcon_Fn = FUNC_NAME(tcon_Fn, Type),
      .tcon_List = FUNC_NAME(tcon_List, Type),
      .tcon_Tuple2 = FUNC_NAME(tcon_Tuple2, Type),
      .tcon_String = FUNC_NAME(tcon_String, Type),
      .func = FUNC_NAME(func, Type),
      .list = FUNC_NAME(list, Type),
      .pair = FUNC_NAME(pair, Type),
  };
}

impl_user_type(Type);

show_user_type(Type)(CharBuff* b, Type x) {
  if (!x) {
    mem_printf(b, "<unknown>");
    return;
  }
  Show(Type) s = trait(Show(Type));
  switch (x->id) {
  case TVAR:
    mem_printf(b, "(TVar %s)", x->tvar.ident);
    /* mem_printf(b, "%s", x->tvar.ident); */
    break;
  case TCON:
    mem_printf(b, "%s", x->tcon.ident);
    break;
  case TAPPLY:
    mem_printf(b, "(TAp ");
    /* mem_printf(b, "("); */
    s.toString(b, x->lhs);
    mem_printf(b, " ");
    s.toString(b, x->rhs);
    mem_printf(b, ")");
    break;
  case TGEN:
    mem_printf(b, "#a%d", x->tgen.n);
    break;
  default:
    assert(0 && "Illegal Type");
    break;
  }
}

// -----------------------------------------------------------------------

static Kind FUNC_NAME(kind, HasKind(Tyvar))(Tyvar t) {
  return t.kind;
}
static Kind FUNC_NAME(kind, HasKind(Tycon))(Tycon t) {
  return t.kind;
}
static Kind FUNC_NAME(kind, HasKind(Type))(Type t) {
  switch (t->id) {
  case TVAR:
    return t->tvar.kind;
  case TCON:
    return t->tcon.kind;
  case TAPPLY:;
    Kind k = FUNC_NAME(kind, HasKind(Type))(t->lhs);
    if (k->id == KFUN) {
      return k->rhs;
    }
    printf("%s\n", trait(Show(Type)).show(t));
    assert(0 && "Invalid Type (illformed kind of type)");
    abort();
  default:
    assert(0 && "Invalid Type");
    abort();
  }
}

HasKind(Tyvar) Trait(HasKind(Tyvar)) {
  return (HasKind(Tyvar)){
      .kind = FUNC_NAME(kind, HasKind(Tyvar)),
  };
}

HasKind(Tycon) Trait(HasKind(Tycon)) {
  return (HasKind(Tycon)){
      .kind = FUNC_NAME(kind, HasKind(Tycon)),
  };
}

HasKind(Type) Trait(HasKind(Type)) {
  return (HasKind(Type)){
      .kind = FUNC_NAME(kind, HasKind(Type)),
  };
}
