/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

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
// expr10 = unary
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

PARSER(Expr) comparison(void); /* 4 */
PARSER(Expr) addsub(void);     /* 6 */
PARSER(Expr) muldiv(void);     /* 7 */
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

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

static String KEYWORDS[] = {
    "let",
    "()",
    "false",
    "true",
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
  return expr3();
}
PARSER(Expr) expr3(void) {
  return expr4();
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
  return unary();
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

PARSER(Expr) fexpr(void) {
  return aexpr();
}

// PARSER(Expr) aexpr(void);
parsec(aexpr, Expr) {
  DO() {
    SCAN(choice(qvar(), ctor(), literal(), paren()), x);
    SCAN(space());
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
    SCAN(space());
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

#endif

C_API_END
