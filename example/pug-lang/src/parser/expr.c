/* -*- coding: utf-8-unix -*- */

#include "parser/expr.h"

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
    if (lhs->id != VAR) {
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
  return choice(lambda(), match(), ifelse(), block(), print(), unary());
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
    A.free(&xs);
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
  return choice(ctor0("()", E.unit()),            /* () */
                ctor0("true", E.boolean(true)),   /* true */
                ctor0("false", E.boolean(false)), /* false */
                varctor()                         /* ADT */
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

// PARSER(Expr) varctor(void);
parsec(varctor, Expr) {
  DO() {
    SCAN(lexme(Identifier()), x);
    RETURN(trait(Expr).var(x));
  }
}

// PARSER(Expr) variable(void);
parsec(variable, Expr) {
  DO() {
    SCAN(lexme(either(identifier(), varsym())), x);
    RETURN(trait(Expr).var(x));
  }
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
    SCAN(some(qvar()), ps);
    SCAN(close_pats);
    SCAN(body, rhs);
    for (Expr* p = A.end(ps); p != A.begin(ps);) {
      rhs = E.lambda(*--p, rhs);
    }
    A.free(&ps);
    RETURN(rhs);
  }
}

// PARSER(Expr) match(void);
parsec(match, Expr) {
  ExprT E = trait(Expr);
  PARSER(String) match_ = lexme(keyword("match"));
  PARSER(char) open_brace = lexme(char1('{'));
  PARSER(char) close_brace = lexme(char1('}'));
  DO() {
    SCAN(match_);
    SCAN(expr0(), e);
    SCAN(open_brace);
    SCAN(alts(), as);
    SCAN(close_brace);
    RETURN(E.match(e, as));
  }
}

// PARSER(List(Alt)) alts(void);
parsec(alts, List(Alt)) {
  ListT(Alt) L = trait(List(Alt));
  PARSER(char) semi = lexme(char1(';'));
  DO() {
    SCAN(alt(), a);
    List(Alt) as = L.cons(a, L.empty);
    List(Alt) bs = as;
    for (;;) {
      SCAN(optional(semi), m);
      if (m.none) {
        break;
      }
      SCAN(optional(alt()), a);
      if (a.none) {
        break;
      }
      bs = bs->tail = L.cons(a.value, L.empty);
    }
    RETURN(as);
  }
}

// PARSER(Alt) alt(void);
parsec(alt, Alt) {
  PARSER(String) arrow = lexme(string1("=>"));
  DO() {
    SCAN(pat(), p);
    SCAN(arrow);
    SCAN(expr0(), e);
    Alt alt = {
        .pats = trait(List(Pat)).cons(p, NULL),
        .e = e,
    };
    RETURN(alt);
  }
}

PARSER(Pat) pat(void) {
  return either(pconstr(), apat());
}

// PARSER(Pat) apat(void);
parsec(apat, Pat) {
  DO() {
    SCAN(choice(wildcard(), pvar(), pctor(), pliteral(), pparen()), x);
    SCAN(blank());
    RETURN(x);
  }
}

// PARSER(Pat) pvar(void);
parsec(pvar, Pat) {
  DO() {
    SCAN(lexme(identifier()), ident);
    RETURN(trait(Pat).PVar(ident));
  }
}

// PARSER(Pat) wildcard(void);
parsec(wildcard, Pat) {
  DO() {
    SCAN(lexme(char1('_')));
    RETURN(trait(Pat).PWildcard());
  }
}

// PARSER(Pat) pctor0(String symbol, Pat p);
parsec(pctor0, String, Pat, Pat) {
  DO() WITH(s, e) {
    SCAN(keyword(s));
    RETURN(e);
  }
}

PARSER(Pat) pctor(void) {
  PatT P = trait(Pat);
  return choice(pctor0("()", P.PLit(LitUnit())),     /* () */
                pctor0("true", P.PLit(LitTrue())),   /* true */
                pctor0("false", P.PLit(LitFalse())), /* false */
                pconstr0()                           /* ADT (arity = 0) */
  );
}

static List(Pat) toListPat(Array(Pat) pa) {
  ArrayT(Pat) A = trait(Array(Pat));
  ListT(Pat) P = trait(List(Pat));
  List(Pat) pats = P.empty;
  for (Pat* p = A.end(pa); p != A.begin(pa);) {
    pats = P.cons(*(--p), pats);
  }
  return pats;
}

// PARSER(Pat) pconstr0(void);
parsec(pconstr0, Pat) {
  PatT P = trait(Pat);
  DO() {
    SCAN(lexme(Identifier()), ident);
    // NOTE: parser cannot check nor determine name and its type-scheme of
    // constructor pattern. These are checked and corrected by type system
    // after.
    Assump a = {.ident = ident, .scheme = {0}};
    RETURN(P.PCon(a, NULL));
  }
}

// PARSER(Pat) pconstr(void);
parsec(pconstr_, Pat) {
  PatT P = trait(Pat);
  DO() {
    SCAN(lexme(Identifier()), ident);
    SCAN(some(apat()), pa);
    ArrayT(Pat) A = trait(Array(Pat));
    // NOTE: parser cannot check nor determine name and its type-scheme of
    // constructor pattern. These are checked and corrected by type system
    // after.
    Assump a = {.ident = ident, .scheme = {0}};
    List(Pat) pats = toListPat(pa);
    A.free(&pa);
    RETURN(P.PCon(a, pats));
  }
}

PARSER(Pat) pconstr(void) {
  return tryp(pconstr_());
}

// PARSER(Pat) pliteral(void);
parsec(pliteral, Pat) {
  PatT P = trait(Pat);
  DO() {
    SCAN(number(), e); /* TODO */
    RETURN(P.PLit(e->literal));
  }
}

// PARSER(Pat) pparen(void);
parsec(pparen, Pat) {
  DO() {
    SCAN(lexme(char1('(')));
    SCAN(pat(), x);
    SCAN(lexme(char1(')')));
    RETURN(x);
  }
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
