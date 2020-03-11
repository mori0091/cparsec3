/* -*- coding: utf-8-unix -*- */
#include <cparsec3/parsec/base/stream.h>

#define S String
#define T_GENERATOR FUNC_NAME(S, generator)

#define Nothing                                                          \
  { .none = true }
#define Just(toks, rest)                                                 \
  { .value.first = (toks), .value.second = (rest) }

struct data {
  int n; /* max length of chunk */
  S input;
  Maybe(Tuple(Tokens(S), S)) expect;
};

static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      {-1, "", Just("", "")},
      {+0, "", Just("", "")},
      {+1, "", Nothing},

      {-1, "abc", Just("", "abc")},
      {+0, "abc", Just("", "abc")},
      {+1, "abc", Just("a", "bc")},
      {+2, "abc", Just("ab", "c")},
      {+3, "abc", Just("abc", "")},
      {+4, "abc", Just("abc", "")},
  };
  if (i < sizeof(ret) / sizeof(ret[0])) {
    return &(ret[i]);
  }
  return NULL;
}

#undef Nothing
#undef Just

#include "./test_stream_takeN.h"
