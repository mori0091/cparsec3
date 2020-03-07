/* -*- coding: utf-8-unix -*- */

#include <cparsec3/parsec/State.h>
#include <cparsec3/parsec/stream.h>

#include <string.h>

impl_State(String);

#define ST State(String)

static bool null(ST s) {
  return trait(Stream(String)).null(s.input);
}

static void updateState(char c, ST* s) {
  switch ((int)(uint8_t)c) {
  case '\0':
    return;
  case '\n':
    s->sourcePos.line++;
    s->sourcePos.column = 1;
    s->lineOffset = s->offset + 1;
    break;
  case '\t':
    s->sourcePos.column =
        ((s->sourcePos.column - 1) / s->tabWidth + 1) * s->tabWidth + 1;
    break;
  default:
    s->sourcePos.column++;
    break;
  }
  s->offset++;
  s->input++;
}

static Maybe(Tuple(Token(ST), ST)) take1(ST s) {
  Stream(String) SS = trait(Stream(String));
  Maybe(Tuple(Token(String), String)) m = SS.take1(s.input);
  if (m.none) {
    return (Maybe(Tuple(Token(ST), ST))){
        .none = true,
    };
  }
  updateState(m.value.e1, &s);
  return (Maybe(Tuple(Token(ST), ST))){
      .value = {{m.value.e1}, {s}},
  };
}

static Maybe(Tuple(Tokens(ST), ST)) takeN(int n, ST s) {
  Stream(String) SS = trait(Stream(String));
  Maybe(Tuple(Tokens(String), String)) m = SS.takeN(n, s.input);
  if (m.none) {
    return (Maybe(Tuple(Tokens(ST), ST))){.none = true};
  }
  const char* cs = m.value.e1;
  while (*cs) {
    updateState(*cs++, &s);
  }
  return (Maybe(Tuple(Tokens(ST), ST))){
      .value = {{m.value.e1}, {s}},
  };
}

static String lineTextOf(ST st) {
  assert(st.lineOffset <= st.offset);
  size_t tabWidth = st.tabWidth;
  CharBuff b = {0};
  int col = 0;
  const char* beg = st.input + st.lineOffset - st.offset;
  for (const char* c = beg; *c && *c != '\n'; c++) {
    if (*c == '\t') {
      int n = tabWidth - (col % tabWidth);
      col += n;
      if (0 < n) {
        mem_printf(&b, "%*s", n, "");
      }
    } else {
      col++;
      mem_printf(&b, "%c", *c);
    }
  }
  if (!b.data || !*b.data) {
    mem_printf(&b, "<empty line>");
  }

  return b.data;
}

static Offset offsetOf(ST s) {
  return s.offset;
}

static void printState(ST s) {
  String lineText = lineTextOf(s);
  int line = s.sourcePos.line;
  int column = s.sourcePos.column;
  String linePrefix = s.linePrefix;
  assert(1 <= line);
  assert(1 <= column);
  int n = snprintf(0, 0, "%d", line);
  assert(1 <= n);
  printf("%d:%d:\n", line, column);
  printf("%*s%s\n", n, "", linePrefix);
  printf("%*d%s%s\n", n, line, linePrefix, lineText);
  printf("%*s%s%*s^\n", n, "", linePrefix, column - 1, "");
}

Stream(ST) Trait(Stream(ST)) {
  Stream(String) SS = trait(Stream(String));
  return (Stream(ST)){
      .null = null,
      .chunkToTokens = SS.chunkToTokens,
      .chunkLength = SS.chunkLength,
      .showTokens = SS.showTokens,
      .take1 = take1,
      .takeN = takeN,

      .offsetOf = offsetOf,
      .printState = printState,
  };
}
