/* -*- coding: utf-8-unix -*- */

#include <cparsec3/stream/stream_string.h>

#include <string.h>

Eq(Token(String)) Trait(Eq(Token(String))) {
  Eq(char) t = trait(Eq(char));
  return *(Eq(Token(String))*)&t;
}

Ord(Token(String)) Trait(Ord(Token(String))) {
  Ord(char) t = trait(Ord(char));
  return *(Ord(Token(String))*)&t;
}

ListT(Token(String)) Trait(List(Token(String))) {
  ListT(char) t = trait(List(char));
  return *(ListT(Token(String))*)&t;
}

Eq(List(Token(String))) Trait(Eq(List(Token(String)))) {
  Eq(List(char)) t = trait(Eq(List(char)));
  return *(Eq(List(Token(String))*))&t;
}

Ord(List(Token(String))) Trait(Ord(List(Token(String)))) {
  Ord(List(char)) t = trait(Ord(List(char)));
  return *(Ord(List(Token(String))*))&t;
}

Iterable(List(Token(String))) Trait(Iterable(List(Token(String)))) {
  Iterable(List(char)) t = trait(Iterable(List(char)));
  return *(Iterable(List(Token(String)))*)&t;
}

ItrT(List(Token(String))) Trait(Itr(List(Token(String)))) {
  ItrT(List(char)) t = trait(Itr(List(char)));
  return *(ItrT(List(Token(String)))*)&t;
}

ArrayT(Token(String)) Trait(Array(Token(String))) {
  ArrayT(char) t = trait(Array(char));
  return *(ArrayT(Token(String))*)&t;
}

Iterable(Array(Token(String))) Trait(Iterable(Array(Token(String)))) {
  Iterable(Array(char)) t = trait(Iterable(Array(char)));
  return *(Iterable(Array(Token(String)))*)&t;
}

ItrT(Array(Token(String))) Trait(Itr(Array(Token(String)))) {
  ItrT(Array(char)) t = trait(Itr(Array(char)));
  return *(ItrT(Array(Token(String)))*)&t;
}

/**
 * Tests whether the stream was empty or not.
 * \param s    a stream
 * \return     true if s was empty, false otherwise.
 */
static bool empty(String s) {
  return !s[0];
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
  typedef Maybe(Tuple(Token(String), String)) R;
  if (empty(s)) {
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
  typedef Maybe(Tuple(Tokens(String), String)) R;
  if (n <= 0) {
    Tokens(String) x = {.length = 0, .value = ""};
    return (R){.value = {{x}, {s}}};
  }
  if (empty(s)) {
    return (R){.none = true};
  } else {
    int m = strnlen(s, n);
    Tokens(String) x = {.length = m, .value = s};
    return (R){.value = {{x}, {s + m}}};
  }
}

/**
 * Returns a set of Stream API functions.
 */
Stream(String) Trait(Stream(String)) {
  return (Stream(String)){.empty = empty, .take1 = take1, .takeN = takeN};
}
