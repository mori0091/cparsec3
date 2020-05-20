/* -*- coding: utf-8-unix -*- */

#include "parser/expr.h"

static String KEYWORDS[] = {
    "let",   "var", "match", "if",  "else", "false", "true",
    "print", "()",  "bool",  "int", "Fn",   "type",
};

static String RESERVED_OP[] = {
    "!", "|", "-", "=", "=>", ";", ":", "::",
};

#define IS_RESERVED(name, tbl)                                           \
  (is_reserved(name, tbl, sizeof(tbl) / sizeof(tbl[0])))

static bool is_reserved(String s, String* tbl, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    if (trait(Eq(String)).eq(s, tbl[i])) {
      return true;
    }
  }
  return false;
}
static bool is_a_keyword(String s) {
  return IS_RESERVED(s, KEYWORDS);
}

static bool is_a_reserved_op(String s) {
  return IS_RESERVED(s, RESERVED_OP);
}

// PARSER(String) identifier0(PARSER(char), PARSER(char));
parsec(identifier0, PARSER(char), PARSER(char), String) {
  DO() WITH(idStart, idLetter) {
    SCAN(idStart, x);
    SCAN(many(idLetter), xs);
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
  return label("identifier",
               tryp(identifier0(identStart(), identLetter())));
}

PARSER(String) Identifier(void) {
  return label("Identifier", tryp(identifier0(upper(), identLetter())));
}

PARSER(char) identStart(void) {
  return either(char1('_'), lower());
}

PARSER(char) identLetter(void) {
  return either(char1('_'), alphaNum());
}

parsec(varsym0, String) {
  DO() {
    SCAN(lexme(char1('(')));
    SCAN(some(symbol()), xs);
    SCAN(blank());
    SCAN(lexme(char1(')')));
    char* cs = mem_realloc(xs.data, xs.length + 1);
    cs[xs.length] = '\0';
    if (is_a_reserved_op(cs)) {
      FAIL("reserved operator");
    }
    RETURN(cs);
  }
}

PARSER(String) varsym(void) {
  return label("operator symbol", tryp(varsym0()));
}

PARSER(char) symbol(void) {
  return choice(char1('!'), char1('%'), char1('&'), char1('='),
                char1('~'), char1(','), char1('-'), char1('^'),
                char1('+'), char1('*'), char1('<'), char1('>'),
                char1('/'), char1('?'));
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
