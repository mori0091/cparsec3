/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#include "Id.h"
#include "Type.h"

typedef struct Context* Context;
typedef struct Expr* Expr;
decl_user_type(Expr);

// -----------------------------------------------------------------------
typedef struct Num {
  int64_t value;
} Num;


enum ExprId {
  /* print statement (for debug purpose) */
  /* NOTE: it's tentative and will be removed when I/O library ready. */
  PRINT,
  // ----
  /* thunk (a (deferable) expr bounded to a context) */
  THUNK,
  /* closure (a lambda abstraction bounded to a context) */
  CLOSURE,
  /* function application (partial application) */
  APPLY,
  /* lambda abstraction */
  LAMBDA,
  /* if else */
  IFELSE,
  /* block */
  BLK,
  /* sequence */
  SEQ,
  /* var (declares a variable and its type) */
  DECLVAR,
  /* type expression (for type annotation) */
  TYPE,
  /* let (define a variable) */
  LET,
  /* assignment */
  ASSIGN,
  /* logical and/or */
  OR,
  AND,
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
  /* constructor */
  CON,
  /* constructor application */
  CAPPLY,
};

struct Expr {
  Type type;
  enum ExprId id;
  union {
    struct {
      Context ctx;
      union {
        /* for THUNK */
        Expr expr;
        /* for CLOSURE */
        Expr lambda;
      };
    };
    struct {
      Expr lhs;
      Expr rhs;
    };
    Num num;
    Id ident;
    Type texpr;
  };
};

// -----------------------------------------------------------------------
typedef struct ExprT {
  Expr (*print)(Expr rhs); /* print statement for debug purpose */
  // ----
  Expr (*thunk)(Context ctx, Expr rhs);   /* thunk */
  Expr (*closure)(Context ctx, Expr rhs); /* closure */
  Expr (*apply)(Expr lhs, Expr rhs);      /* function application */
  Expr (*lambda)(Expr lhs, Expr rhs);     /* lambda abstraction */
  Expr (*ifelse)(Expr c, Expr t, Expr e); /* if then else */
  Expr (*block)(Expr rhs);                /* block */
  Expr (*seq)(Expr lhs, Expr rhs);        /* list of statements */
  Expr (*declvar)(Expr lhs, Expr rhs);    /* variable declaration */
  Expr (*let)(Expr lhs, Expr rhs);        /* variable definition */
  Expr (*assign)(Expr lhs, Expr rhs);     /* assignment */
  Expr (*logic_or)(Expr lhs, Expr rhs);   /* logical or */
  Expr (*logic_and)(Expr lhs, Expr rhs);  /* logical and */
  Expr (*eq)(Expr lhs, Expr rhs);         /* equality */
  Expr (*neq)(Expr lhs, Expr rhs);        /* nonequality */
  Expr (*le)(Expr lhs, Expr rhs);         /* less than or equal to */
  Expr (*lt)(Expr lhs, Expr rhs);         /* less than */
  Expr (*gt)(Expr lhs, Expr rhs);         /* greater than */
  Expr (*ge)(Expr lhs, Expr rhs);         /* greater than or equal to */
  Expr (*add)(Expr lhs, Expr rhs);        /* arithmetic addition */
  Expr (*sub)(Expr lhs, Expr rhs);        /* arithmetic subtraction */
  Expr (*mul)(Expr lhs, Expr rhs);        /* arithmetic multiplication */
  Expr (*div)(Expr lhs, Expr rhs);        /* arithmetic division */
  Expr (*mod)(Expr lhs, Expr rhs);        /* arithmetic reminder */
  Expr (*neg)(Expr rhs);                  /* arithmetic negation */
  Expr (*not)(Expr rhs);   /* logical not / bitwise complement */
  Expr (*num)(Num x);      /* number */
  Expr (*var)(Id x);       /* variable */
  Expr (*boolean)(bool b); /* true / false */
  Expr (*unit)(void);      /* () */
  Expr (*type)(Type t);    /* type annotation */
  Expr (*con)(Id c);       /* constructor */
  Expr (*capply)(Expr lhs, Expr rhs); /* constructor application */
} ExprT;

ExprT Trait(Expr);
