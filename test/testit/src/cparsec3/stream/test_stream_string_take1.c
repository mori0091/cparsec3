/* -*- coding: utf-8-unix -*- */
#include <cparsec3/parsec/stream.h>

#define S String
#define T_GENERATOR FUNC_NAME(S, generator)

#define Nothing                                                          \
  { .none = true }
#define Just(tok, rest)                                                  \
  { .value.first = (tok), .value.second = (rest) }

struct data {
  S input;
  Maybe(Tuple(Token(S), S)) expect;
};

static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      {"abc", Just('a', "bc")},
      {"bc", Just('b', "c")},
      {"c", Just('c', "")},
      {"", Nothing},
  };
  if (i < sizeof(ret) / sizeof(ret[0])) {
    return &(ret[i]);
  }
  return NULL;
}

#undef Nothing
#undef Just

#include "./test_stream_take1.h"
