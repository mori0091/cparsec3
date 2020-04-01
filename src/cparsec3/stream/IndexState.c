/* -*- coding: utf-8-unix -*- */

#include <cparsec3/parsec/stream.h>

#include <string.h>

impl_IndexState(String);

#define S String
#define ST IndexState(S)

static bool null(ST s) {
  return trait(Stream(S)).null(s.input);
}

static void update(Token(S) c, ST* s) {
  assert(c);
  s->input++;
  s->index++;
}

static Maybe(Tuple(Token(ST), ST)) take1(ST s) {
  Stream(S) SS = trait(Stream(S));
  Maybe(Tuple(Token(S), S)) m = SS.take1(s.input);
  if (m.none) {
    return (Maybe(Tuple(Token(ST), ST))){
        .none = true,
    };
  }
  update(m.value.e1, &s);
  return (Maybe(Tuple(Token(ST), ST))){
      .value = {{m.value.e1}, {s}},
  };
}

static Maybe(Tuple(Tokens(ST), ST)) takeN(int n, ST s) {
  Stream(S) SS = trait(Stream(S));
  Maybe(Tuple(Tokens(S), S)) m = SS.takeN(n, s.input);
  if (m.none) {
    return (Maybe(Tuple(Tokens(ST), ST))){.none = true};
  }
  const char* cs = m.value.e1;
  while (*cs) {
    update(*cs++, &s);
  }
  return (Maybe(Tuple(Tokens(ST), ST))){
      .value = {{m.value.e1}, {s}},
  };
}

static Offset offsetOf(ST s) {
  return (Offset)s.index;
}

static void stringifyState(CharBuff* b, ST s) {
  mem_printf(b, "at index %zu\n", s.index);
}

Stream(ST) Trait(Stream(ST)) {
  Stream(S) SS = trait(Stream(S));
  return (Stream(ST)){
      .null = null,
      .chunkToTokens = SS.chunkToTokens,
      .chunkLength = SS.chunkLength,
      .showTokens = SS.showTokens,
      .take1 = take1,
      .takeN = takeN,

      .offsetOf = offsetOf,
      .stringifyState = stringifyState,
  };
}
