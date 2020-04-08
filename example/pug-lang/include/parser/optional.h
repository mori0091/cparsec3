/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../my_parsec.h"

C_API_BEGIN

PARSER(Maybe(char)) optional(PARSER(char) p);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

parsec(optional, PARSER(char), Maybe(char)) {
  DO() WITH(p) {
    SCAN(count_min_max(0, 1, p), m);
    if (!m.length) {
      RETURN((Maybe(char)){.none = true});
    }
    char x = m.data[0];
    trait(Array(char)).free(&m);
    RETURN((Maybe(char)){.value = x});
  }
}

#endif

C_API_END
