/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../core/core.h"

#define Match(S, T) CONCAT(Match, S, T)

#define TOKEN(S, T) CONCAT(cparsec_token, S, T)
#define TOKEN_FN(S, T) CONCAT(cparsec_token_fn, S, T)
#define TOKEN_ARG(S, T) CONCAT(cparsec_token_arg, S, T)
#define declare_token(S, T)                                              \
  typedef_Maybe(T);                                                      \
  typedef Maybe(T) (*Match(S, T))(Token(S));                             \
  Parsec(S, T) TOKEN(S, T)(Match(S, T) match, Hints(S) expecting)

#define define_token(S, T)                                               \
  typedef struct {                                                       \
    Match(S, T) match;                                                   \
    Hints(S) expecting;                                                  \
  } TOKEN_ARG(S, T);                                                     \
  static PSResult(S, T) TOKEN_FN(S, T)(void* arg, PState(S) state) {     \
    TOKEN_ARG(S, T)* p = arg;                                            \
    Stream(S) stream = cparsec_module(Stream(S));                        \
    Maybe(Tuple(Token(S), S)) r0 = stream.take1(state.input);            \
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
  /* token(match, expecting)*/                                           \
  Parsec(S, T) TOKEN(S, T)(Match(S, T) match, Hints(S) expecting) {      \
    TOKEN_ARG(S, T)* arg = malloc(sizeof(TOKEN_ARG(S, T)));              \
    arg->match = match;                                                  \
    arg->expecting = expecting;                                          \
    return (Parsec(S, T)){.run = TOKEN_FN(S, T), .arg = arg};            \
  }                                                                      \
  END_OF_STATEMENTS
