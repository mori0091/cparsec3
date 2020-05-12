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
// decl   = let | declvar | decltype
// let    = "let" variable "=" expr0
// declvar = "var" variable type_annotation
// decltype = "type" simpletype "=" constrs
//
// simpletype = qtctor {tvar}
// constrs = constr {"|" constr}
//
// type_annotation = ":" texpr
// texpr  = tlambda | btype
// tlambda = "|" atype {atype} "|" atype
// btype  = [btype] atype
// atype  = tctor
//        | tvar
//        | tparen
//
// tctor  = "()" | "bool" | "int" | qtctor
// qtctor = Identifier
// tvar   = identifier
// tparen = "(" texpr ")"
//
// constr  = Identifier {texpr}
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
// ctor    = "()" | "true" | "false" | varctor
// literal = number
// paren   = (" expr ")"
//
// varctor       = Identifier
// variable    = identifier | varsym
//
// Identifier  = upper{identLetter}
// identifier  = identStart{identLetter}
// identStart  = "_" | letter
// identLetter = "_" | alphaNum
//
// varsym      = "(" symbol{symbol} ")"
// symbol      = "!" | "%" | "&" | "=" | "~" | "|"
//             | "-" | "^" | "+" | "*" | "<" | ">"
//             | "/" | "?"

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

PARSER(Expr) varctor(void);
PARSER(Expr) variable(void);

/* an identifer start w/ uppercase letter */
PARSER(String) Identifier(void);
/* an identifier start w/ lowercase letter or '_' */
PARSER(String) identifier(void);
PARSER(char) identStart(void);
PARSER(char) identLetter(void);

PARSER(String) varsym(void);
PARSER(char) symbol(void);

PARSER(String) keyword(String s);

PARSER(Expr) lambda(void);
PARSER(Expr) pat(void);

PARSER(Expr) ifelse(void);

PARSER(Expr) block(void);
PARSER(Expr) stmts(void);
PARSER(Expr) stmt(void);

PARSER(Expr) decl(void);
PARSER(Expr) let(void);
PARSER(Expr) declvar(void);
PARSER(Expr) decltype(void);
PARSER(Type) simpletype(void);
PARSER(Expr) constrs(Type datatype);
PARSER(Expr) constr(Type datatype);

PARSER(Expr) type_annotation(void);

PARSER(Type) texpr(void);
PARSER(Type) btype(void);
PARSER(Type) atype(void);
PARSER(Type) tlambda(void);
PARSER(Type) tctor(void);
PARSER(Type) qtctor(void);
PARSER(Type) tvar(void);
PARSER(Type) tparen(void);

C_API_END
