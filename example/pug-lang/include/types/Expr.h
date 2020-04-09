/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

typedef struct Expr* Expr;

typedef struct Num {
  int value;
} Num;

enum ExprType {
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  NEG,
  NOT,
  NUM,
};

struct Expr {
  enum ExprType type;
  union {
    struct {
      Expr lhs;
      Expr rhs;
    };
    Num num;
  };
};

typedef struct ExprT {
  Expr (*add)(Expr lhs, Expr rhs);
  Expr (*sub)(Expr lhs, Expr rhs);
  Expr (*mul)(Expr lhs, Expr rhs);
  Expr (*div)(Expr lhs, Expr rhs);
  Expr (*mod)(Expr lhs, Expr rhs);
  Expr (*neg)(Expr rhs);
  Expr (*not)(Expr rhs);
  Expr (*num)(Num x);
} ExprT;

ExprT Trait(Expr);

decl_user_type(Expr);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

static Expr Expr_New(void) {
  return (Expr)mem_malloc(sizeof(struct Expr));
}

static Expr Expr_Binary(enum ExprType type, Expr lhs, Expr rhs) {
  Expr e = Expr_New();
  e->type = type;
  e->lhs = lhs;
  e->rhs = rhs;
  return e;
}

static Expr Expr_Unary(enum ExprType type, Expr rhs) {
  Expr e = Expr_New();
  e->type = type;
  e->lhs = 0;
  e->rhs = rhs;
  return e;
}

static void Expr_showBinary(CharBuff* b, String tag, Expr lhs, Expr rhs) {
  Show(Expr) s = trait(Show(Expr));
  mem_printf(b, "(%s ", tag);
  s.toString(b, lhs);
  mem_printf(b, " ");
  s.toString(b, rhs);
  mem_printf(b, ")");
}

static void Expr_showUnary(CharBuff* b, String tag, Expr rhs) {
  Show(Expr) s = trait(Show(Expr));
  mem_printf(b, "(%s ", tag);
  s.toString(b, rhs);
  mem_printf(b, ")");
}

static Expr FUNC_NAME(add, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(ADD, lhs, rhs);
}
static Expr FUNC_NAME(sub, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(SUB, lhs, rhs);
}
static Expr FUNC_NAME(mul, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(MUL, lhs, rhs);
}
static Expr FUNC_NAME(div, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(DIV, lhs, rhs);
}
static Expr FUNC_NAME(mod, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(MOD, lhs, rhs);
}
static Expr FUNC_NAME(neg, Expr)(Expr rhs) {
  return Expr_Unary(NEG, rhs);
}
static Expr FUNC_NAME(not, Expr)(Expr rhs) {
  return Expr_Unary(NOT, rhs);
}
static Expr FUNC_NAME(num, Expr)(Num x) {
  Expr e = Expr_New();
  e->type = NUM;
  e->num = x;
  return e;
}

ExprT Trait(Expr) {
  return (ExprT){
      .add = FUNC_NAME(add, Expr),
      .sub = FUNC_NAME(sub, Expr),
      .mul = FUNC_NAME(mul, Expr),
      .div = FUNC_NAME(div, Expr),
      .mod = FUNC_NAME(mod, Expr),
      .neg = FUNC_NAME(neg, Expr),
      .not = FUNC_NAME(not, Expr),
      .num = FUNC_NAME(num, Expr),
  };
}

impl_user_type(Expr);

show_user_type(Expr)(CharBuff* b, Expr x) {
  switch (x->type) {
  case ADD:
    Expr_showBinary(b, "Add", x->lhs, x->rhs);
    break;
  case SUB:
    Expr_showBinary(b, "Sub", x->lhs, x->rhs);
    break;
  case MUL:
    Expr_showBinary(b, "Mul", x->lhs, x->rhs);
    break;
  case DIV:
    Expr_showBinary(b, "Div", x->lhs, x->rhs);
    break;
  case MOD:
    Expr_showBinary(b, "Mod", x->lhs, x->rhs);
    break;
  case NEG:
    Expr_showUnary(b, "Neg", x->rhs);
    break;
  case NOT:
    Expr_showUnary(b, "Not", x->rhs);
    break;
  case NUM:
    mem_printf(b, "(Num %d)", x->num.value);
    break;
  default:
    assert(0 && "Illegal Expr");
  }
}

#endif
