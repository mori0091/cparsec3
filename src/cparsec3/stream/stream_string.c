/* -*- coding: utf-8-unix -*- */

#include <cparsec3/stream/stream_string.h>

#include <string.h>

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
Stream(String) CPARSEC_MODULE(Stream(String))(void) {
  return (Stream(String)){.empty = empty, .take1 = take1, .takeN = takeN};
}
