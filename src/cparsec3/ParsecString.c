/* -*- coding: utf-8-unix -*- */

#include <cparsec3/ParsecString.h>

#include <string.h>

CPARSEC_DEFINE_ALL();

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
static Maybe(Tupple(Token(String), String)) take1(String s) {
  typedef Maybe(Tupple(Token(String), String)) R;
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
static Maybe(Tupple(Tokens(String), String)) takeN(int n, String s) {
  typedef Maybe(Tupple(Tokens(String), String)) R;
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

// ----

#define TOKEN(S, T) CONCAT(cparsec_token, S, T)
#define TOKEN_FN(S, T) CONCAT(cparsec_token_fn, S, T)
#define TOKEN_ARG(S, T) CONCAT(cparsec_token_arg, S, T)
#define declare_token(S, T)                                              \
  typedef_Maybe(T);                                                      \
  Parsec(S, T) TOKEN(S, T)(Maybe(T)(*match)(Token(S)),                   \
                           LList(ErrorItem(S)) expecting)

#define Hints(S) LList(ErrorItem(S))
#define define_token(S, T)                                               \
  typedef struct {                                                       \
    Maybe(T) (*match)(Token(S));                                         \
    Hints(S) expecting;                                                  \
  } TOKEN_ARG(S, T);                                                     \
  static PSResult(S, T) TOKEN_FN(S, T)(void* arg, PState(S) state) {     \
    TOKEN_ARG(S, T)* p = arg;                                            \
    Stream(S) stream = cparsec_module(Stream(S));                        \
    Maybe(Tupple(Token(S), S)) r0 = stream.take1(state.input);           \
    if (r0.none) {                                                       \
      return UNEXPECTED_EOM(S, T)(state, p->expecting);                  \
    }                                                                    \
    state.input = r0.value.second;                                       \
    state.offset++;                                                      \
    Token(S) actual = r0.value.first;                                    \
    Maybe(T) r = p->match(actual);                                       \
    if (r.none) {                                                        \
      return UNEXPECTED_TOKEN(S, T)(state, actual, p->expecting);        \
    }                                                                    \
    return PARSE_OK(S, T)(state, r.value);                               \
  }                                                                      \
  Parsec(S, T)                                                           \
      TOKEN(S, T)(Maybe(T)(*match)(Token(S)), Hints(S) expecting) {      \
    TOKEN_ARG(S, T)* arg = malloc(sizeof(TOKEN_ARG(S, T)));              \
    arg->match = match;                                                  \
    arg->expecting = expecting;                                          \
    return (Parsec(S, T)){.run = TOKEN_FN(S, T), .arg = arg};            \
  }                                                                      \
  END_OF_STATEMENTS

declare_token(String, char);
define_token(String, char);
