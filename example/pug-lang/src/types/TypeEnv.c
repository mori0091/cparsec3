/* -*- coding: utf-8-unix -*- */

#include "types/TypeEnv.h"
#include "types/Context.h"

static Type judge1(Context ctx, Expr e) {
  TypeEnv G = trait(TypeEnv);
  ContextT C = trait(Context);
  TypeT T = trait(Type);
  Eq(Type) E = trait(Eq(Type));
  Type Unit = T.tcon_unit();
  Type Bool = T.tcon_bool();
  Type Int = T.tcon_int();
  switch (e->kind) {
  case PRINT:
    return Unit;
  case THUNK:
    return G.judge(e->ctx, e->expr);
  case CLOSURE:
    return G.judge(e->ctx, e->lambda);
  case APPLY:
    return NULL;
  case LAMBDA:
    return NULL;
  case IFELSE: {
    if (E.neq(G.judge(ctx, e->lhs), Bool)) {
      return NULL;
    }
    Type tl = G.judge(ctx, e->rhs->lhs);
    Type tr = G.judge(ctx, e->rhs->rhs);
    if (E.eq(tl, tr)) {
      return tl;
    }
    return NULL;
  }
  case BLK:
    return G.judge(ctx, e->rhs);
  case SEQ:
    G.judge(ctx, e->lhs);
    return G.judge(ctx, e->rhs);
  case DECLVAR:
    return G.judge(ctx, e->rhs);
  case TYPE:
    return e->texpr;
  case LET: {
    Type tr = G.judge(ctx, e->rhs);
    MapEntry* m = C.map.lookup_local(ctx, e->lhs->var.ident);
    if (m && m->type && !m->e) {
      // if the variable is locally declared but not defined yet, type
      // must be same.
      if (E.eq(m->type, tr)) {
        return m->type;
      }
      return NULL;
    } else {
      // if the previous definiton exists, it will be shadowed.
      return tr;
    }
  }
  case ASSIGN: {
    Type tr = G.judge(ctx, e->rhs);
    Type tl = G.judge(ctx, e->lhs);
    if (E.eq(tl, tr)) {
      return tr;
    }
    return NULL;
  }
  case OR:
  case AND: {
    Type tl = G.judge(ctx, e->lhs);
    Type tr = G.judge(ctx, e->rhs);
    if (E.eq(tl, Bool) && E.eq(tl, tr)) {
      return Bool;
    }
    return NULL;
  }
  case EQ:
  case NEQ:
  case LE:
  case LT:
  case GT:
  case GE: {
    Type tl = G.judge(ctx, e->lhs);
    Type tr = G.judge(ctx, e->rhs);
    if (E.eq(tl, tr)) {
      return Bool;
    }
    return NULL;
  }
  case ADD:
  case SUB:
  case MUL:
  case DIV:
  case MOD: {
    Type tl = G.judge(ctx, e->lhs);
    Type tr = G.judge(ctx, e->rhs);
    if (E.eq(tl, Int) && E.eq(tl, tr)) {
      return Int;
    }
    return NULL;
  }
  case NEG: {
    Type tr = G.judge(ctx, e->rhs);
    if (E.eq(tr, Int)) {
      return Int;
    }
    return NULL;
  }
  case NOT: {
    Type tr = G.judge(ctx, e->rhs);
    if (E.eq(tr, Bool)) {
      return Bool;
    }
    if (E.eq(tr, Int)) {
      return Int;
    }
    return NULL;
  }
  case VAR: {
    MapEntry* m = C.map.lookup(ctx, e->var.ident);
    if (m) {
      return m->type;
    }
    return NULL;
  }
  case NUM:
    return Int;
  case FALSE:
  case TRUE:
    return Bool;
  case UNIT:
    return Unit;
  default:
    return NULL;
  }
}

static Type judge(Context ctx, Expr e) {
  assert(e && "Null pointer");
  Type t = judge1(ctx, e);
  if (trait(Eq(Type)).neq(t, e->type)) {
    printf(">>> %s : %s\n", trait(Show(Expr)).show(e),
           trait(Show(Type)).show(t));
  }
  e->type = t;
  return t;
}

TypeEnv Trait(TypeEnv) {
  return (TypeEnv){
      .judge = judge,
  };
}
