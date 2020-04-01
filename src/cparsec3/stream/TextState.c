/* -*- coding: utf-8-unix -*- */

#include <cparsec3/parsec/stream.h>

#include <string.h>

impl_TextState(String);

#define S String
#define ST TextState(S)

typedef struct TextPosition TextPosition;
struct TextPosition {
  int line;
  int column;
};

static bool null(ST s) {
  return trait(Stream(S)).null(s.input);
}

static void update(Token(S) c, ST* s) {
  assert(c);
  s->input++;
  if (c == '\n') {
    s->line++;
    s->headOfLine = s->input;
  }
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

static TextPosition lineColumn(ST st, size_t tabWidth) {
  assert(st.headOfLine <= st.input);
  int col = 0;
  for (const Token(S)* c = st.headOfLine; c != st.input; c++) {
    assert(*c && *c != '\n');
    if (*c == '\t') {
      int n = tabWidth - (col % tabWidth);
      col += n;
    } else {
      col++;
    }
  }
  return (TextPosition){.line = st.line, .column = col + 1};
}

static void stringifyLineText(CharBuff* b, ST st, size_t tabWidth) {
  assert(st.headOfLine <= st.input);
  int col = 0;
  for (const Token(S)* c = st.headOfLine; *c && *c != '\n'; c++) {
    if (*c == '\t') {
      int n = tabWidth - (col % tabWidth);
      col += n;
      if (0 < n) {
        mem_printf(b, "%*s", n, "");
      }
    } else {
      col++;
      mem_printf(b, "%c", *c);
    }
  }
  if (!col) {
    mem_printf(b, "<empty line>");
  }
  mem_printf(b, "\n");
}

static Offset offsetOf(ST s) {
  return trait(Stream(S)).offsetOf(s.input);
}

static void stringifyState(CharBuff* b, ST s) {
  size_t TAB_WIDTH = 8;
  String LINE_PREFIX = " | ";
  TextPosition pos = lineColumn(s, TAB_WIDTH);
  assert(1 <= pos.line);
  assert(1 <= pos.column);
  int n = snprintf(0, 0, "%d", pos.line);
  assert(1 <= n);
  mem_printf(b, "%d:%d:\n", pos.line, pos.column);
  mem_printf(b, "%*s%s\n", n, "", LINE_PREFIX);
  mem_printf(b, "%*d%s", n, pos.line, LINE_PREFIX);
  stringifyLineText(b, s, TAB_WIDTH);
  mem_printf(b, "%*s%s%*s^\n", n, "", LINE_PREFIX, pos.column - 1, "");
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
