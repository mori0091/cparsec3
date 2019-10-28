/* -*- coding: utf-8-unix -*- */
#include <cparsec3/stream/stream_string.h>

#define S String
#define T_GENERATOR FUNC_NAME(S, generator)

#define Nothing                                                          \
  { .none = true }
#define Just(len, toks, rest)                                            \
  {                                                                      \
    .value.first = {.length = (len), .value = (toks)},                   \
    .value.second = (rest)                                               \
  }
#define may_follos_something ".."

struct data {
  int n; /* max length of chunk */
  S input;
  Maybe(Tuple(Tokens(S), S)) expect;
};

static void* T_GENERATOR(size_t i) {
  static struct data ret[] = {
      {-1, "", Just(0, "" may_follos_something, "")},
      {+0, "", Just(0, "" may_follos_something, "")},
      {+1, "", Nothing},

      {-1, "abc", Just(0, "" may_follos_something, "abc")},
      {+0, "abc", Just(0, "" may_follos_something, "abc")},
      {+1, "abc", Just(1, "a" may_follos_something, "bc")},
      {+2, "abc", Just(2, "ab" may_follos_something, "c")},
      {+3, "abc", Just(3, "abc" may_follos_something, "")},
      {+4, "abc", Just(3, "abc" may_follos_something, "")},
  };
  if (i < sizeof(ret) / sizeof(ret[0])) {
    return &(ret[i]);
  }
  return NULL;
}

#undef Nothing
#undef Just
#undef may_follos_something

#include "./test_stream_takeN.h"
