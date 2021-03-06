/* -*- coding: utf-8-unix -*- */

#include "parser/expr.h"
#include "constructs.h"

PARSER(Expr) decl(void) {
  return choice(let(), declvar(), declADT());
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

// PARSER(Expr) declADT(void);
parsec(declADT, Expr) {
  DO() {
    SCAN(lexme(keyword("type")));
    SCAN(simpletype(), lhs);
    SCAN(lexme(char1('=')));
    SCAN(constrs(lhs), rhs);
    RETURN(rhs);
  }
}

// PARSER(Type) simpletype(void);
parsec(simpletype, Type) {
  ArrayT(Type) A = trait(Array(Type));
  DO() {
    SCAN(Identifier(), c);
    SCAN(many(atype()), targs);
    Type lhs = makeSimpleType(c, targs);
    A.free(&targs);
    RETURN(lhs);
  }
}

// PARSER(Expr) constrs(Type datatype);
parsec(constrs, Type, Expr) {
  ExprT E = trait(Expr);
  PARSER(Maybe(char)) sep = optional(lexme(char1('|')));
  DO() WITH(datatype) {
    PARSER(Expr) p = constr(datatype);
    SCAN(p, xs);
    SCAN(sep, m);
    if (m.none) {
      RETURN(xs);
    }
    SCAN(constrs(datatype), ys);
    xs->rhs = E.seq(xs->rhs, ys);
    RETURN(xs);
  }
}

// PARSER(Expr) constr(Type datatype);
parsec(constr, Type, Expr) {
  ArrayT(Type) A = trait(Array(Type));
  DO() WITH(datatype) {
    SCAN(Identifier(), name);
    SCAN(many(atype()), args);
    Expr e = defineConstructor(name, args, datatype);
    A.free(&args);
    RETURN(e);
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
  return choice(simpletype(), tlambda(), btype());
}

// PARSER(Type) tlambda(void);
parsec(tlambda, Type) {
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
    rhs = makeTypeSig(ps, rhs);
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
      lhs = t.TAp(lhs, *e++);
    }
    A.free(&xs);
    RETURN(lhs);
  }
}

// PARSER(Type) simpletype0(void);
parsec(simpletype0, Type) {
  ArrayT(Type) A = trait(Array(Type));
  DO() {
    SCAN(Identifier(), c);
    Type lhs = makeSimpleType(c, A.empty);
    RETURN(lhs);
  }
}

PARSER(Type) atype(void) {
  return choice(tctor(), tvar(), tparen(), simpletype0());
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
               choice(tctor_unit(), tctor_bool(), tctor_int()));
}

parsec(tvar0, Type) {
  DO() {
    SCAN(identifier(), x);
    RETURN(trait(Type).TVar((Tyvar){x, trait(Kind).Star()}));
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
