/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

C_API_BEGIN

PARSER(char) lexme(PARSER(char) p);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

parsec(lexme, PARSER(char), char) {
  DO() WITH(p) {
    SCAN(p, x);
    SCAN(space());
    RETURN(x);
  }
}

#endif

C_API_END
