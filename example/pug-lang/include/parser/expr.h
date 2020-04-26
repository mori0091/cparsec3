/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "blank.h"
#include "lexme.h"
#include "optional.h"

#include "number.h"

C_API_BEGIN

// expr   = assign
// assign = expr0 {"=" expr0}
// expr0  = expr1
// expr1  = expr2
// expr2  = expr3 {"||" expr3}
// expr3  = expr4 {"&&" expr4}
// expr4  = expr5 [("==" | "!=" | "<=" | "<" | ">" | ">=") expr5]
// expr5  = expr6
// expr6  = expr7 {("+" | "-") expr7}
// expr7  = expr8 {("*" | "/") expr8}
// expr8  = expr9
// expr9  = expr10
// expr10 = lambda
//        | ifelse
//        | block
//        | print
//        | unary
//
// lambda = "|" pat {pat} "|" expr0
// pat    = variable
//
// ifelse = "if" expr0 "{" stmts "}" "else" (if_expr | "{" stmts "}")
//
// block  = "{" stmts "}"
// stmts  = stmt {";" [stmt]}
// stmt   = decl | expr
//
// decl   = let
// let    = "let" variable "=" expr0
//
// print  = "print" fexpr           (TODO: remove when I/O library ready.)
// unary  = [("+" | "-" | "!")] fexpr
// fexpr  = [fexpr] aexpr
// aexpr  = qvar
//        | ctor
//        | literal
//        | paren
//
// qvar    = variable
// ctor    = "()" | "true" | "false"
// literal = number
// paren   = (" expr ")"
//
// variable    = identifier
// identifier  = identStart{identLetter}
// identStart  = "_" | letter
// identLetter = "_" | alphaNum

PARSER(Expr) expr(void);
PARSER(Expr) assign(void);

PARSER(Expr) expr0(void);
PARSER(Expr) expr1(void);
PARSER(Expr) expr2(void);
PARSER(Expr) expr3(void);
PARSER(Expr) expr4(void);
PARSER(Expr) expr5(void);
PARSER(Expr) expr6(void);
PARSER(Expr) expr7(void);
PARSER(Expr) expr8(void);
PARSER(Expr) expr9(void);
PARSER(Expr) expr10(void);

PARSER(Expr) logic_or(void);   /* 2 */
PARSER(Expr) logic_and(void);  /* 3 */
PARSER(Expr) comparison(void); /* 4 */
PARSER(Expr) addsub(void);     /* 6 */
PARSER(Expr) muldiv(void);     /* 7 */

/* print statement (for debug purpose) */
/* NOTE: it's tentative and will be removed when I/O library ready. */
PARSER(Expr) print(void);

PARSER(Expr) unary(void);
PARSER(Expr) fexpr(void); /* funcion application */
PARSER(Expr) aexpr(void);
PARSER(Expr) qvar(void); /* qualified variable */
PARSER(Expr) ctor(void); /* data constructor */
PARSER(Expr) literal(void);
PARSER(Expr) paren(void);

PARSER(Expr) variable(void);
PARSER(String) identifier(void);
PARSER(char) identStart(void);
PARSER(char) identLetter(void);

PARSER(String) keyword(String s);

PARSER(Expr) lambda(void);
PARSER(Expr) pat(void);

PARSER(Expr) ifelse(void);

PARSER(Expr) block(void);
PARSER(Expr) stmts(void);
PARSER(Expr) stmt(void);

PARSER(Expr) decl(void);
PARSER(Expr) let(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

static String KEYWORDS[] = {
    "let", "if", "else", "()", "false", "true", "print",
};

static bool is_a_keyword(String s) {
  for (size_t i = 0; i < sizeof(KEYWORDS) / sizeof(String); ++i) {
    if (trait(Eq(String)).eq(s, KEYWORDS[i])) {
      return true;
    }
  }
  return false;
}

PARSER(Expr) expr(void) {
  return assign();
}

// PARSER(Expr) assign(void);
parsec(assign, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = expr0();
  PARSER(char) op = lexme(char1('='));
  DO() {
    SCAN(p, lhs);
    if (lhs->kind != VAR) {
      RETURN(lhs);
    }
    SCAN(optional(op), m);
    if (m.none) {
      RETURN(lhs);
    }
    SCAN(p, rhs);
    RETURN(E.assign(lhs, rhs));
  }
}

PARSER(Expr) expr0(void) {
  return expr1();
}
PARSER(Expr) expr1(void) {
  return expr2();
}
PARSER(Expr) expr2(void) {
  return logic_or();
}
PARSER(Expr) expr3(void) {
  return logic_and();
}
PARSER(Expr) expr4(void) {
  return comparison();
}
PARSER(Expr) expr5(void) {
  return expr6();
}
PARSER(Expr) expr6(void) {
  return addsub();
}
PARSER(Expr) expr7(void) {
  return muldiv();
}
PARSER(Expr) expr8(void) {
  return expr9();
}
PARSER(Expr) expr9(void) {
  return expr10();
}
PARSER(Expr) expr10(void) {
  return choice(lambda(), ifelse(), block(), print(), unary());
}

// PARSER(Expr) logic_or(void);
parsec(logic_or, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = expr3();
  PARSER(String) op = lexme(string1("||"));
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op), m);
      if (m.none) {
        break;
      }
      /* SCAN(p, rhs); */
      SCAN(expr2(), rhs);
      lhs = E.logic_or(lhs, rhs);
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) logic_and(void);
parsec(logic_and, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = expr4();
  PARSER(String) op = lexme(string1("&&"));
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op), m);
      if (m.none) {
        break;
      }
      /* SCAN(p, rhs); */
      SCAN(expr3(), rhs);
      lhs = E.logic_and(lhs, rhs);
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) comparison(void);
parsec(comparison, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = expr5();
  PARSER(String)
  op = lexme(choice(string1("=="), string1("!="), /* eq, neq */
                    string1("<="), string1(">="), /* le, ge */
                    string1("<"), string1(">"))   /* lt, gt */
  );
  DO() {
    SCAN(p, lhs);
    SCAN(optional(op), m);
    if (m.none) {
      RETURN(lhs);
    }
    SCAN(p, rhs);
    if (g_eq("==", m.value)) {
      RETURN(E.eq(lhs, rhs));
    }
    if (g_eq("!=", m.value)) {
      RETURN(E.neq(lhs, rhs));
    }
    if (!m.value[1]) {
      RETURN(((m.value[0] == '<') ? E.lt : E.gt)(lhs, rhs));
    } else {
      RETURN(((m.value[0] == '<') ? E.le : E.ge)(lhs, rhs));
    }
  }
}

// PARSER(Expr) addsub(void);
parsec(addsub, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = expr7();
  PARSER(char) op = lexme(either(char1('+'), char1('-')));
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op), m);
      if (m.none) {
        break;
      }
      SCAN(p, rhs);
      lhs = ((m.value == '+') ? E.add : E.sub)(lhs, rhs);
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) muldiv(void);
parsec(muldiv, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = expr8();
  PARSER(char) op = lexme(choice(char1('*'), char1('/'), char1('%')));
  DO() {
    SCAN(p, lhs);
    for (;;) {
      SCAN(optional(op), m);
      if (m.none) {
        break;
      }
      SCAN(p, rhs);
      switch (m.value) {
      case '*':
        lhs = E.mul(lhs, rhs);
        break;
      case '/':
        lhs = E.div(lhs, rhs);
        break;
      case '%':
        lhs = E.mod(lhs, rhs);
        break;
      default:
        FAIL("Unexpected behaviour");
        break;
      }
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) print(void);
parsec(print, Expr) {
  ExprT E = trait(Expr);
  PARSER(String) f = lexme(keyword("print"));
  PARSER(Expr) p = fexpr();
  DO() {
    SCAN(f);
    SCAN(p, rhs);
    RETURN(E.print(rhs));
  }
}

// PARSER(Expr) unary(void);
parsec(unary, Expr) {
  ExprT E = trait(Expr);
  PARSER(Expr) p = fexpr();
  PARSER(char) op = lexme(choice(char1('+'), char1('-'), char1('!')));
  DO() {
    SCAN(optional(op), m);
    if (m.none) {
      SCAN(p, rhs);
      RETURN(rhs);
    }
    SCAN(unary(), rhs);
    switch (m.value) {
    case '+':
      RETURN(rhs);
    case '-':
      RETURN(E.neg(rhs));
    case '!':
      RETURN(E.not(rhs));
    default:
      FAIL("Unexpected behaviour");
    }
  }
}

// PARSER(Expr) fexpr(void)
parsec(fexpr, Expr) {
  ExprT E = trait(Expr);
  ArrayT(Expr) A = trait(Array(Expr));
  PARSER(Expr) p = aexpr();
  DO() {
    SCAN(some(p), xs);
    Expr* e = A.begin(xs);
    Expr lhs = *e++;
    while (e != A.end(xs)) {
      lhs = E.apply(lhs, *e++);
    }
    RETURN(lhs);
  }
}

// PARSER(Expr) aexpr(void);
parsec(aexpr, Expr) {
  DO() {
    SCAN(choice(qvar(), ctor(), literal(), paren()), x);
    SCAN(blank());
    RETURN(x);
  }
}

PARSER(Expr) qvar(void) {
  return variable();
}

// PARSER(Expr) ctor0(String symbol, Expr e);
parsec(ctor0, String, Expr, Expr) {
  DO() WITH(s, e) {
    SCAN(keyword(s));
    RETURN(e);
  }
}

PARSER(Expr) ctor(void) {
  ExprT E = trait(Expr);
  return choice(ctor0("()", E.unit()),           /* () */
                ctor0("true", E.boolean(true)),  /* true */
                ctor0("false", E.boolean(false)) /* false */
  );
}

PARSER(Expr) literal(void) {
  return number();
}

// PARSER(Expr) paren(void);
parsec(paren, Expr) {
  DO() {
    SCAN(lexme(char1('(')));
    SCAN(expr(), x);
    SCAN(lexme(char1(')')));
    RETURN(x);
  }
}

// PARSER(Expr) variable(void);
parsec(variable, Expr) {
  DO() {
    SCAN(lexme(identifier()), x);
    RETURN(trait(Expr).var((Var){x}));
  }
}

// PARSER(String) identifier0(void);
parsec(identifier0, String) {
  DO() {
    SCAN(identStart(), x);
    SCAN(many(identLetter()), xs);
    SCAN(blank());
    size_t len = 1 + xs.length;
    char* cs = mem_malloc(len + 1);
    cs[0] = x;
    memmove(cs + 1, xs.data, xs.length);
    cs[len] = 0;
    g_free(xs);
    if (is_a_keyword(cs)) {
      FAIL("keyword");
    }
    RETURN(cs);
  }
}

PARSER(String) identifier(void) {
  return label("identifier", tryp(identifier0()));
}

PARSER(char) identStart(void) {
  return either(char1('_'), letter());
}

PARSER(char) identLetter(void) {
  return either(char1('_'), alphaNum());
}

// PARSER(String) keyword0(String s);
parsec(keyword0, String, String) {
  DO() WITH(s) {
    SCAN(string1(s));
    SCAN(optional(identLetter()), m);
    if (!m.none) {
      FAIL("?");
    }
    RETURN(s);
  }
}

PARSER(String) keyword(String s) {
  return tryp(keyword0(s));
}

// PARSER(Expr) lambda(void);
parsec(lambda, Expr) {
  ExprT E = trait(Expr);
  ArrayT(Expr) A = trait(Array(Expr));
  PARSER(char) open_pats = lexme(char1('|'));
  PARSER(char) close_pats = open_pats;
  PARSER(Expr) body = expr();
  DO() {
    SCAN(open_pats);
    SCAN(some(pat()), ps);
    SCAN(close_pats);
    SCAN(body, rhs);
    for (Expr* p = A.end(ps); p != A.begin(ps);) {
      rhs = E.lambda(*--p, rhs);
    }
    A.free(&ps);
    RETURN(rhs);
  }
}

PARSER(Expr) pat(void) {
  return variable();
}

// PARSER(Expr) braces(PARSER(Expr) p);
parsec(braces, PARSER(Expr), Expr) {
  PARSER(char) open_brace = lexme(char1('{'));
  PARSER(char) close_brace = lexme(char1('}'));
  DO() WITH(p) {
    SCAN(open_brace);
    SCAN(p, x);
    SCAN(close_brace);
    RETURN(x);
  }
}

// PARSER(Expr) ifelse(void);
parsec(ifelse, Expr) {
  ExprT E = trait(Expr);
  PARSER(String) if_ = lexme(keyword("if"));
  PARSER(String) else_ = lexme(keyword("else"));
  PARSER(Expr) blk = block();
  DO() {
    SCAN(if_);
    SCAN(expr0(), x);
    SCAN(blk, y);
    SCAN(else_);
    SCAN(either(blk, ifelse()), z);
    RETURN(E.ifelse(x, y, z));
  }
}

// PARSER(Expr) block(void);
parsec(block, Expr) {
  ExprT E = trait(Expr);
  DO() {
    SCAN(braces(stmts()), x);
    RETURN(E.block(x));
  }
}

// PARSER(Expr) stmts(void);
parsec(stmts, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) semi = lexme(char1(';'));
  DO() {
    SCAN(stmt(), lhs);
    SCAN(optional(semi), m);
    if (m.none) {
      RETURN(lhs);
    }
    SCAN(optional(stmts()), rhs);
    if (rhs.none) {
      RETURN(lhs);
    }
    RETURN(E.seq(lhs, rhs.value));
  }
}

PARSER(Expr) stmt(void) {
  return choice(decl(), expr());
}

PARSER(Expr) decl(void) {
  return let();
}

// PARSER(Expr) let(void);
parsec(let, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) op = lexme(char1('='));
  DO() {
    SCAN(lexme(keyword("let")));
    SCAN(variable(), lhs);
    SCAN(op);
    SCAN(expr0(), rhs);
    RETURN(E.let(lhs, rhs));
  }
}

#endif

C_API_END
