/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

#include "optional.h"

C_API_BEGIN

PARSER(None) blank(void);
PARSER(None) lineComment(void);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

// PARSER(None) blank(void);
parsec(blank, None) {
  PARSER(Maybe(None)) comment = optional(lineComment());
  DO() {
    SCAN(space());
    for (;;) {
      SCAN(comment, m);
      if (m.none) {
        break;
      }
    }
    RETURN((None){0});
  }
}

// PARSER(None) lineComment(void);
parsec(lineComment, None) {
  PARSER(String) lineCommentStart = string1("//");
  // PARSER(Maybe(String)) endOfLine = optional(eol());
  PARSER(Maybe(char)) endOfLine = optional(newline());
  PARSER(char) any = anySingle();
  DO() {
    SCAN(lineCommentStart);
    for (;;) {
      SCAN(endOfLine, m);
      if (!m.none) {
        break;
      }
      SCAN(any);
    }
    SCAN(space());
    RETURN((None){0});
  }
}

#endif

C_API_END
