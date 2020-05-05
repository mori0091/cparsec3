/* -*- coding: utf-8-unix -*- */

#include "parser/expr.h"

PARSER(Expr) decl(void) {
  return either(let(), declvar());
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

// PARSER(Expr) declvar(void);
parsec(declvar, Expr) {
  ExprT E = trait(Expr);
  DO() {
    SCAN(lexme(keyword("var")));
    SCAN(variable(), lhs);
    SCAN(type_annotation(), rhs);
    RETURN(E.declvar(lhs, rhs));
  }
}

// PARSER(Expr) type_annotation(void);
parsec(type_annotation, Expr) {
  ExprT E = trait(Expr);
  PARSER(char) op = lexme(char1(':'));
  DO() {
    SCAN(op);
    SCAN(texpr(), ty);
    RETURN(E.type(ty));
  }
}

PARSER(Type) texpr(void) {
  return either(tlambda(), btype());
}

// PARSER(Type) tlambda(void);
parsec(tlambda, Type) {
  TypeT t = trait(Type);
  ArrayT(Type) A = trait(Array(Type));
  PARSER(char) open_pats = lexme(char1('|'));
  PARSER(char) close_pats = open_pats;
  PARSER(Type) arg = atype();
  PARSER(Type) body = texpr();
  DO() {
    SCAN(open_pats);
    SCAN(some(arg), ps);
    SCAN(close_pats);
    SCAN(body, rhs);
    for (Type* p = A.end(ps); p != A.begin(ps);) {
      rhs = t.funcType(*--p, rhs);
    }
    A.free(&ps);
    RETURN(rhs);
  }
}

// PARSER(Type) btype(void);
parsec(btype, Type) {
  TypeT t = trait(Type);
  ArrayT(Type) A = trait(Array(Type));
  PARSER(Type) p = atype();
  DO() {
    SCAN(some(p), xs);
    Type* e = A.begin(xs);
    Type lhs = *e++;
    while (e != A.end(xs)) {
      lhs = t.tapply(lhs, *e++);
    }
    A.free(&xs);
    RETURN(lhs);
  }
}

PARSER(Type) atype(void) {
  return choice(tctor(), tvar(), tparen());
}

parsec(tctor_unit, Type) {
  DO() {
    SCAN(lexme(keyword("()")));
    RETURN(trait(Type).tcon_unit());
  }
}

parsec(tctor_bool, Type) {
  DO() {
    SCAN(lexme(keyword("bool")));
    RETURN(trait(Type).tcon_bool());
  }
}

parsec(tctor_int, Type) {
  DO() {
    SCAN(lexme(keyword("int")));
    RETURN(trait(Type).tcon_int());
  }
}

PARSER(Type) tctor(void) {
  return label("type constructor",
               choice(tctor_unit(), tctor_bool(), tctor_int(), qtctor()));
}

// PARSER(Type) qtctor(void);
parsec(qtctor, Type) {
  DO() {
    SCAN(Identifier(), x);
    RETURN(trait(Type).tcon((TCon){x}));
  }
}

parsec(tvar0, Type) {
  DO() {
    SCAN(identifier(), x);
    RETURN(trait(Type).tvar((TVar){x}));
  }
}

PARSER(Type) tvar(void) {
  return label("type variable", tvar0());
}

// PARSER(Type) tparen(void);
parsec(tparen, Type) {
  DO() {
    SCAN(lexme(char1('(')));
    SCAN(texpr(), x);
    SCAN(lexme(char1(')')));
    RETURN(x);
  }
}
