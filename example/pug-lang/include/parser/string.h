/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

C_API_BEGIN

PARSER(char) escaped_char(void);
PARSER(Array(char)) double_quoted_string(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

parsec(escaped_char, char) {
  DO () {
    SCAN(anySingleBut((char)'"'), c);
    if (c != '\\') {
      RETURN(c);
    }
    SCAN(anySingle(), x);
    switch (x) {
    case '0':
      RETURN('\0');
    case 'a':
      RETURN('\a');
    case 'b':
      RETURN('\b');
    case 'f':
      RETURN('\f');
    case 'n':
      RETURN('\n');
    case 'r':
      RETURN('\r');
    case 't':
      RETURN('\t');
    case 'v':
      RETURN('\v');
    case '\\':
    case '\'':
    case '"':
    case '?':
      RETURN(x);
    default:
      RETURN(x);
    }
  }
}

// PARSER(Array(char)) double_quoted_string0(void);
parsec(double_quoted_string0, Array(char)) {
  DO() {
    SCAN(char1((char)'"'));
    SCAN(many(escaped_char()), s);
    SCAN(char1((char)'"'));
    RETURN(s);
  }
}

PARSER(Array(char)) double_quoted_string(void) {
  return label("double-quoted string", tryp(double_quoted_string0()));
}

#endif

C_API_END
