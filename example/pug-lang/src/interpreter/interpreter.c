/* -*- coding: utf-8-unix -*- */

#include "interpreter/interpreter.h"

// -----------------------------------------------------------------------
static EvalResult eval_expr1(Context ctx, Expr x);
static EvalResult eval_expr(Context ctx, Expr x);

Interpreter(Expr) Trait(Interpreter(Expr)) {
  return (Interpreter(Expr)){
      .eval = eval_expr,
  };
}

// -----------------------------------------------------------------------
#define EVAL(_ctx_, _x_, _var_)                                          \
  EvalResult _var_ = eval_expr(_ctx_, _x_);                              \
  if (!_var_.success) {                                                  \
    return _var_;                                                        \
  }

#define RETURN_OK(_x_) return EVAL_RESULT_OK(_x_)
#define RETURN_DEFERED(_ctx_, _x_) return EVAL_RESULT_DEFERED(_ctx_, _x_)
#define RETURN_ERR(_msg_) return EVAL_RESULT_ERR(_msg_)

#define INFIX_COMPARISON_OP(_ctx_, _op_, _a_, _b_)                       \
  do {                                                                   \
    EVAL(_ctx_, _a_, lhs);                                               \
    EVAL(_ctx_, _b_, rhs);                                               \
    switch (lhs.ok->id) {                                                \
    case LITERAL: {                                                      \
      switch (lhs.ok->literal.id) {                                      \
      case LIT_INTEGER: {                                                \
        int64_t x = lhs.ok->literal.num.value;                           \
        int64_t y = rhs.ok->literal.num.value;                           \
        RETURN_OK(trait(Expr).boolean(x _op_ y));                        \
      }                                                                  \
      case LIT_FALSE:                                                    \
      case LIT_TRUE:                                                     \
      case LIT_UNIT: {                                                   \
        enum LiteralId x = lhs.ok->literal.id;                           \
        enum LiteralId y = rhs.ok->literal.id;                           \
        RETURN_OK(trait(Expr).boolean(x _op_ y));                        \
      }                                                                  \
      }                                                                  \
    }                                                                    \
    case CLOSURE:                                                        \
    case CCON: {                                                         \
      bool b = lhs.ok _op_ rhs.ok;                                       \
      RETURN_OK(trait(Expr).boolean(b));                                 \
    }                                                                    \
    default:                                                             \
      RETURN_ERR("Type error");                                          \
    }                                                                    \
  } while (0)

#define INFIX_OP(_ctx_, _op_, _a_, _b_)                                  \
  do {                                                                   \
    EVAL(_ctx_, _a_, lhs);                                               \
    EVAL(_ctx_, _b_, rhs);                                               \
    int64_t x = lhs.ok->literal.num.value;                               \
    int64_t y = rhs.ok->literal.num.value;                               \
    RETURN_OK(trait(Expr).num((Num){x _op_ y}));                         \
  } while (0)

#define DIV_MOD_OP(_ctx_, _op_, _a_, _b_)                                \
  do {                                                                   \
    EVAL(_ctx_, _a_, lhs);                                               \
    EVAL(_ctx_, _b_, rhs);                                               \
    int64_t x = lhs.ok->literal.num.value;                               \
    int64_t y = rhs.ok->literal.num.value;                               \
    if (y == 0) {                                                        \
      RETURN_ERR("Division by zero");                                    \
    }                                                                    \
    RETURN_OK(trait(Expr).num((Num){x _op_ y}));                         \
  } while (0)

// -----------------------------------------------------------------------
static EvalResult eval_apply(Context ctx, Expr x) {
  EVAL(ctx, x->lhs, f);
  if (f.ok->id != CLOSURE) {
    RETURN_ERR("function application");
  }
  Expr v = f.ok->lambda->lhs;    // (Var v)
  Expr body = f.ok->lambda->rhs; // body
  ContextT C = trait(Context);
  Context c = C.branch(f.ok->ctx);
  ExprT E = trait(Expr);
  C.map.put(c, v->ident, E.thunk(ctx, x->rhs));
  RETURN_DEFERED(c, body);
}

static EvalResult eval_lambda(Context ctx, Expr x) {
  ContextT C = trait(Context);
  ExprT E = trait(Expr);
  RETURN_OK(E.closure(C.nested(ctx), x));
}

static EvalResult eval_con(Context ctx, Expr x) {
  ExprT E = trait(Expr);
  RETURN_OK(E.ccon(ctx, x));
}

static bool match(Context c, Expr x, Pat pat) {
  ContextT C = trait(Context);
  switch (pat->id) {
  case PWILDCARD:
    return true;
  case PVAR:
    C.map.put(c, pat->ident, x);
    return true;
  case PLITERAL:
    if (x->id != LITERAL) {
      return false;
    }
    return trait(Eq(Literal)).eq(pat->literal, x->literal);
  case PCON:
    if (x->id == CON) {
      EvalResult r = eval_expr(c, x);
      if (!r.success) {
        return false;
      }
      x = r.ok;
    }
    if (x->id != CCON) {
      return false;
    }
    if (trait(Eq(String)).neq(pat->a.ident, x->con->ident)) {
      return false;
    }
    List(Expr) args = x->con->args;
    for (List(Pat) pats = pat->pats; pats; pats = pats->tail) {
      EvalResult r = eval_expr(x->ctx, args->head);
      if (!r.success) {
        return false;
      }
      if (!match(c, r.ok, pats->head)) {
        return false;
      }
      args = args->tail;
    }
    return true;
  default:
    return false;
  }
}

static EvalResult eval_match(Context ctx, Expr x) {
  EVAL(ctx, x->match_arg, arg);
  Context c = trait(Context).branch(ctx);
  for (List(Alt) alts = x->alts; alts; alts = alts->tail) {
    Alt alt = alts->head;
    /* TODO what should we do for pats->tail? */
    if (match(c, arg.ok, alt.pats->head)) {
      RETURN_DEFERED(c, alt.e);
    }
  }
  RETURN_ERR("pattern matching failed");
}

static EvalResult eval_ifelse(Context ctx, Expr x) {
  ContextT C = trait(Context);
  // make implicit block scope that encloses whole if~else block
  Context c = C.branch(ctx);
  EVAL(c, x->lhs, cond);
  Expr then_blk = x->rhs->lhs;
  Expr else_blk = x->rhs->rhs;
  if (cond.ok->literal.id == LIT_TRUE) {
    RETURN_DEFERED(c, then_blk);
  } else {
    RETURN_DEFERED(c, else_blk);
  }
}

static EvalResult eval_block(Context ctx, Expr x) {
  ContextT C = trait(Context);
  RETURN_DEFERED(C.branch(ctx), x->rhs);
}

static EvalResult eval_seq(Context ctx, Expr x) {
  EVAL(ctx, x->lhs, lhs);
  RETURN_DEFERED(ctx, x->rhs);
}

static EvalResult eval_let(Context ctx, Expr x) {
  ContextT C = trait(Context);
  ExprT E = trait(Expr);
  assert(x->lhs->id == VAR);
  // if the previous definiton exists, it will be shadowed.
  C.map.put(ctx, x->lhs->ident, E.thunk(ctx, x->rhs));
  RETURN_OK(x->rhs);
}

static EvalResult eval_declvar(Context ctx, Expr x) {
  ContextT C = trait(Context);
  assert(x->lhs->id == VAR);
  assert(x->rhs->id == TYPE);
  // if the previous definiton exists, it will be shadowed.
  C.map.put(ctx, x->lhs->ident, NULL);
  RETURN_OK(x->rhs);
}

static EvalResult eval_logical_AND_OR(Context ctx, Expr x) {
  EVAL(ctx, x->lhs, lhs);
  assert(lhs.ok->id == LITERAL);
  if (lhs.ok->literal.id == (x->id == OR ? LIT_TRUE : LIT_FALSE)) {
    RETURN_OK(lhs.ok);
  }
  EVAL(ctx, x->rhs, rhs);
  RETURN_OK(rhs.ok);
}

static EvalResult eval_print(Context ctx, Expr x) {
  ExprT E = trait(Expr);
  EVAL(ctx, x->rhs, rhs);
  printf("%s\n", trait(Show(Expr)).show(rhs.ok));
  RETURN_OK(E.unit());
}

static EvalResult eval_negate(Context ctx, Expr x) {
  ExprT E = trait(Expr);
  Expr y = E.sub(E.num((Num){0}), x->rhs);
  EVAL(ctx, y, rhs);
  RETURN_OK(rhs.ok);
}

static EvalResult eval_not(Context ctx, Expr x) {
  ExprT E = trait(Expr);
  EVAL(ctx, x->rhs, rhs);
  assert(rhs.ok->id == LITERAL);
  switch (rhs.ok->literal.id) {
  case LIT_INTEGER:
    RETURN_OK(E.num((Num){~rhs.ok->literal.num.value}));
  case LIT_TRUE:
    RETURN_OK(E.boolean(false));
  case LIT_FALSE:
    RETURN_OK(E.boolean(true));
  default:
    RETURN_ERR("Type error");
  }
}

static EvalResult eval_var(Context ctx, Expr x) {
  ContextT C = trait(Context);
  MapEntry* m = C.map.lookup(ctx, x->ident);
  if (!m || !m->e) {
    RETURN_ERR("Undefined variable");
  }
  EVAL(ctx, m->e, val);
  m->e = val.ok; /* replace with evaluated value */
  RETURN_OK(val.ok);
}

// -----------------------------------------------------------------------
static EvalResult eval_expr1(Context ctx, Expr x) {
  switch (x->id) {
  case APPLY:
    return eval_apply(ctx, x);
  case LAMBDA:
    return eval_lambda(ctx, x);
  case MATCH:
    return eval_match(ctx, x);
  case IFELSE:
    return eval_ifelse(ctx, x);
  case BLK:
    return eval_block(ctx, x);
  case SEQ:
    return eval_seq(ctx, x);
  case LET:
    return eval_let(ctx, x);
  case OR:
  case AND:
    return eval_logical_AND_OR(ctx, x);
  case EQ:
    INFIX_COMPARISON_OP(ctx, ==, x->lhs, x->rhs);
  case NEQ:
    INFIX_COMPARISON_OP(ctx, !=, x->lhs, x->rhs);
  case LE:
    INFIX_COMPARISON_OP(ctx, <=, x->lhs, x->rhs);
  case LT:
    INFIX_COMPARISON_OP(ctx, <, x->lhs, x->rhs);
  case GT:
    INFIX_COMPARISON_OP(ctx, >, x->lhs, x->rhs);
  case GE:
    INFIX_COMPARISON_OP(ctx, >=, x->lhs, x->rhs);
  case ADD:
    INFIX_OP(ctx, +, x->lhs, x->rhs);
  case SUB:
    INFIX_OP(ctx, -, x->lhs, x->rhs);
  case MUL:
    INFIX_OP(ctx, *, x->lhs, x->rhs);
  case DIV:
    DIV_MOD_OP(ctx, /, x->lhs, x->rhs);
  case MOD:
    DIV_MOD_OP(ctx, %, x->lhs, x->rhs);
  case PRINT:
    return eval_print(ctx, x);
  case NEG:
    return eval_negate(ctx, x);
  case NOT:
    return eval_not(ctx, x);
  case VAR:
    return eval_var(ctx, x);
  case LITERAL:
  case CLOSURE:
  case THUNK:
    RETURN_OK(x);
  case DECLVAR:
    return eval_declvar(ctx, x);
  case TYPE:
    RETURN_OK(x); /* TODO */
  case CON:
    return eval_con(ctx, x);
  case CCON:
    RETURN_OK(x);
  default:
    RETURN_ERR("Illegal Expr");
  }
}

static inline bool is_defered(EvalResult r) {
  return r.ctx != 0;
}

static inline bool is_thunk(EvalResult r) {
  return r.success && r.ok->id == THUNK;
}

static EvalResult eval_expr(Context ctx, Expr x) {
  EvalResult r = eval_expr1(ctx, x);
  while (is_defered(r) || is_thunk(r)) {
    if (is_defered(r)) {
      r = eval_expr1(r.ctx, r.ok);
    }
    if (is_thunk(r)) {
      Expr thunk = r.ok;
      r = eval_expr1(thunk->ctx, thunk->expr);
      if (r.success) {
        thunk->expr = r.ok;
      }
    }
  }
  return r;
}
