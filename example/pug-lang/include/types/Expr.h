/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#include "Type.h"

typedef struct Expr* Expr;

typedef struct Num {
  int value;
} Num;

typedef struct Var {
  String ident;
} Var;

enum ExprId {
  /* sequence */
  SEQ,
  /* let */
  LET,
  /* assignment */
  ASSIGN,
  /* equality */
  EQ,
  NEQ,
  /* ordered */
  LE,
  LT,
  GT,
  GE,
  /* addsub */
  ADD,
  SUB,
  /* muldiv */
  MUL,
  DIV,
  MOD,
  /* unary */
  NEG,
  NOT,
  /* variable */
  VAR,
  /* number */
  NUM,
  /* false, true */
  FALSE,
  TRUE,
  /* () */
  UNIT,
};

struct Expr {
  Type type;
  enum ExprId kind;
  union {
    struct {
      Expr lhs;
      Expr rhs;
    };
    Num num;
    Var var;
  };
};

typedef struct ExprT {
  Expr (*seq)(Expr lhs, Expr rhs);
  Expr (*let)(Expr lhs, Expr rhs);
  Expr (*assign)(Expr lhs, Expr rhs);
  Expr (*eq)(Expr lhs, Expr rhs);
  Expr (*neq)(Expr lhs, Expr rhs);
  Expr (*le)(Expr lhs, Expr rhs);
  Expr (*lt)(Expr lhs, Expr rhs);
  Expr (*gt)(Expr lhs, Expr rhs);
  Expr (*ge)(Expr lhs, Expr rhs);
  Expr (*add)(Expr lhs, Expr rhs);
  Expr (*sub)(Expr lhs, Expr rhs);
  Expr (*mul)(Expr lhs, Expr rhs);
  Expr (*div)(Expr lhs, Expr rhs);
  Expr (*mod)(Expr lhs, Expr rhs);
  Expr (*neg)(Expr rhs);
  Expr (*not)(Expr rhs);
  Expr (*num)(Num x);
  Expr (*var)(Var x);
  Expr (*boolean)(bool b);
  Expr (*unit)(void);
} ExprT;

ExprT Trait(Expr);

decl_user_type(Expr);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

static Expr Expr_New(void) {
  Expr e = (Expr)mem_malloc(sizeof(struct Expr));
  e->type = TYPE(undetermined);
  return e;
}

static Expr Expr_Binary(enum ExprId kind, Expr lhs, Expr rhs) {
  Expr e = Expr_New();
  e->kind = kind;
  e->lhs = lhs;
  e->rhs = rhs;
  return e;
}

static Expr Expr_Unary(enum ExprId kind, Expr rhs) {
  Expr e = Expr_New();
  e->kind = kind;
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

static Expr FUNC_NAME(seq, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(SEQ, lhs, rhs);
}
static Expr FUNC_NAME(let, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(LET, lhs, rhs);
}
static Expr FUNC_NAME(assign, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(ASSIGN, lhs, rhs);
}
static Expr FUNC_NAME(eq, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(EQ, lhs, rhs);
}
static Expr FUNC_NAME(neq, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(NEQ, lhs, rhs);
}
static Expr FUNC_NAME(le, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(LE, lhs, rhs);
}
static Expr FUNC_NAME(lt, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(LT, lhs, rhs);
}
static Expr FUNC_NAME(gt, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(GT, lhs, rhs);
}
static Expr FUNC_NAME(ge, Expr)(Expr lhs, Expr rhs) {
  return Expr_Binary(GE, lhs, rhs);
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
static Expr FUNC_NAME(var, Expr)(Var x) {
  Expr e = Expr_New();
  e->kind = VAR;
  e->var = x;
  return e;
}
static Expr FUNC_NAME(num, Expr)(Num x) {
  Expr e = Expr_New();
  e->type = TYPE(int);
  e->kind = NUM;
  e->num = x;
  return e;
}
static Expr FUNC_NAME(boolean, Expr)(bool b) {
  static struct Expr T = {
      .type = {.id = TypeId(bool)},
      .kind = TRUE,
  };
  static struct Expr F = {
      .type = {.id = TypeId(bool)},
      .kind = FALSE,
  };
  return (b ? &T : &F);
}
static Expr FUNC_NAME(unit, Expr)(void) {
  static struct Expr e = {
      .type = {.id = TypeId(unit)},
      .kind = UNIT,
  };
  return &e;
}

ExprT Trait(Expr) {
  return (ExprT){
      .seq = FUNC_NAME(seq, Expr),
      .let = FUNC_NAME(let, Expr),
      .assign = FUNC_NAME(assign, Expr),
      .eq = FUNC_NAME(eq, Expr),
      .neq = FUNC_NAME(neq, Expr),
      .le = FUNC_NAME(le, Expr),
      .lt = FUNC_NAME(lt, Expr),
      .gt = FUNC_NAME(gt, Expr),
      .ge = FUNC_NAME(ge, Expr),
      .add = FUNC_NAME(add, Expr),
      .sub = FUNC_NAME(sub, Expr),
      .mul = FUNC_NAME(mul, Expr),
      .div = FUNC_NAME(div, Expr),
      .mod = FUNC_NAME(mod, Expr),
      .neg = FUNC_NAME(neg, Expr),
      .not = FUNC_NAME(not, Expr),
      .var = FUNC_NAME(var, Expr),
      .num = FUNC_NAME(num, Expr),
      .boolean = FUNC_NAME(boolean, Expr),
      .unit = FUNC_NAME(unit, Expr),
  };
}

impl_user_type(Expr);

show_user_type(Expr)(CharBuff* b, Expr x) {
  switch (x->kind) {
  case SEQ:
    Expr_showBinary(b, "Seq", x->lhs, x->rhs);
    break;
  case LET:
    Expr_showBinary(b, "Let", x->lhs, x->rhs);
    break;
  case ASSIGN:
    Expr_showBinary(b, "Assign", x->lhs, x->rhs);
    break;
  case EQ:
    Expr_showBinary(b, "Eq", x->lhs, x->rhs);
    break;
  case NEQ:
    Expr_showBinary(b, "Neq", x->lhs, x->rhs);
    break;
  case LE:
    Expr_showBinary(b, "Le", x->lhs, x->rhs);
    break;
  case LT:
    Expr_showBinary(b, "Lt", x->lhs, x->rhs);
    break;
  case GT:
    Expr_showBinary(b, "Gt", x->lhs, x->rhs);
    break;
  case GE:
    Expr_showBinary(b, "Ge", x->lhs, x->rhs);
    break;
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
  case VAR:
    mem_printf(b, "(Var %s)", x->var.ident);
    break;
  case NUM:
    mem_printf(b, "%d", x->num.value);
    break;
  case TRUE:
    mem_printf(b, "true");
    break;
  case FALSE:
    mem_printf(b, "false");
    break;
  case UNIT:
    mem_printf(b, "()");
    break;
  default:
    assert(0 && "Illegal Expr");
    break;
  }
}

#endif
