/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>
#include <cparsec3/stream/stream_string.h>

#include <cparsec3/parsec/parsestate.h>
#include <cparsec3/parsec/posstate.h>

#include <string.h>

impl_PosState(String);
impl_ParseState(String);

/**
 * Tests whether the stream was empty or not.
 * \param s    a stream
 * \return     true if s was empty, false otherwise.
 */
static bool null(String s) {
  assert(s && "Null pointer");
  return !s[0];
}

static int chunkLength(Tokens(String) chk) {
  assert(chk && "Null pointer");
  size_t n = strnlen(chk, SIZE_MAX / 2);
  return (int)n;
}

static List(Token(String)) chunkToTokens(Tokens(String) chk) {
  assert(chk && "Null pointer");
  return trait(List(Token(String)))
      .from_array(chunkLength(chk), (char*)chk);
}

static String showTokens(List(Token(String)) ts) {
  ListT(Token(String)) L = trait(List(Token(String)));
  assert(!L.null(ts));
  CharBuff b = {0};
  if (L.null(L.tail(ts))) {
    trait(Show(Token(String))).toString(&b, L.head(ts));
    return b.data;
  }
  mem_printf(&b, "\"");
  while (!L.null(ts)) {
    quote_char(&b, L.head(ts));
    ts = L.tail(ts);
  }
  mem_printf(&b, "\"");
  return b.data;
}

/**
 * Takes a token from the stream.
 *
 * take1() takes a token from the stream `s` then returns a `Maybe` object
 * that wraps the token and the rest of the stream. If `s` was empty,
 * returns a `Maybe` object that has nothing.
 *
 * \param s   a stream
 * \return    a Maybe object that wraps:
 *            - the token and the rest of stream, or
 *            - nothing (if `s` was empty)
 */
static Maybe(Tuple(Token(String), String)) take1(String s) {
  assert(s && "Null pointer");
  typedef Maybe(Tuple(Token(String), String)) R;
  if (null(s)) {
    return (R){.none = true};
  } else {
    return (R){.value = {{s[0]}, {s + 1}}};
  }
}

/**
 * Takes a chunk of tokens from the stream.
 *
 * takeN() takes a chunk of tokens of `n` length at most from the stream
 * `s` then returns a `Maybe` object that wraps the chunk and the rest of
 * the stream. If `n` was less than or equals to 0, returns a `Maybe`
 * object that wraps 0 length chunk and the `s`. If `n` was greater than 0
 * and `s` was empty, returns a `Maybe` object that has nothing.
 *
 * \param n   max length of chunk
 * \param s   a stream
 * \return    a Maybe object that wraps:
 *            - 0 length chunk and `s` (if `n <= 0`),
 *            - the chunk of tokens and the rest of stream, or
 *            - nothing (if `n > 0` and `s` was empty)
 */
static Maybe(Tuple(Tokens(String), String)) takeN(int n, String s) {
  assert(s && "Null pointer");
  typedef Maybe(Tuple(Tokens(String), String)) R;
  if (n <= 0) {
    return (R){.value = {{""}, {s}}};
  }
  if (null(s)) {
    return (R){.none = true};
  } else {
    int m = strnlen(s, n);
    char* chk = mem_malloc(m + 1);
    assert(chk && "no memory");
    if (0 < m) {
      memcpy(chk, s, m);
    }
    chk[m] = '\0';
    return (R){.value = {{chk}, {s + m}}};
  }
}

static PosState(String) advanceTo(Offset o, PosState(String) pst) {
  assert(pst.offset <= o && "cannot advance to backward position");
  size_t tabWidth = pst.tabWidth;
  int line = pst.sourcePos.line;
  int column = pst.sourcePos.column;
  Offset lineOffset = pst.lineOffset;
  const char* c = pst.input;
  for (Offset p = pst.offset; p < o; p++, c++) {
    assert(*c && "unexpected end of input");
    switch ((int)(uint8_t)*c) {
    case '\n':
      line++;
      column = 1;
      lineOffset = p + 1;
      break;
    case '\t':
      column = ((column - 1) / tabWidth + 1) * tabWidth + 1;
      break;
    default:
      column++;
      break;
    }
  }
  pst.input = c;
  pst.offset = o;
  pst.lineOffset = lineOffset;
  pst.sourcePos.line = line;
  pst.sourcePos.column = column;
  return pst;
}

static String lineTextOf(PosState(String) pst) {
  assert(pst.lineOffset <= pst.offset);
  size_t tabWidth = pst.tabWidth;
  CharBuff b = {0};
  int col = 0;
  const char* beg = pst.input + pst.lineOffset - pst.offset;
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

static void printState(ParseState(String) s) {
  PosState(String) pst = advanceTo(s.offset, s.posState);
  trait(PosState(String)).print(lineTextOf(pst), pst);
}

/**
 * Returns a set of Stream API functions.
 */
Stream(String) Trait(Stream(String)) {
  return (Stream(String)){
      .null = null,
      .chunkToTokens = chunkToTokens,
      .chunkLength = chunkLength,
      .showTokens = showTokens,
      .take1 = take1,
      .takeN = takeN,

      .printState = printState,
  };
}
