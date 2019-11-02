/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../core/core.h"
#include "combinator.h"

#define Match(S, T) TYPE_NAME(Match, S, T)

#define declare_token(S, T)                                              \
  /* typedef_Maybe(T); */                                                \
  typedef Maybe(T) (*Match(S, T))(Token(S));                             \
  declare_parsec(S, T, token, Match(S, T), Hints(S))

#define define_token(S, T)                                               \
  define_parsec(S, T, token, Match(S, T), Hints(S)) {                    \
    Match(S, T) match = arg->_1;                                         \
    Hints(S) expecting = arg->_2;                                        \
    Stream(S) stream = trait(Stream(S));                                 \
    Maybe(Tuple(Token(S), S)) r0 = stream.take1(state.input);            \
    if (r0.none) {                                                       \
      return UNEXPECTED_EOM(S, T)(state, expecting);                     \
    }                                                                    \
    state.input = r0.value.second;                                       \
    state.offset++;                                                      \
    Token(S) actual = r0.value.first;                                    \
    Maybe(T) r = match(actual);                                          \
    if (r.none) {                                                        \
      return UNEXPECTED_TOKEN(S, T)(state, actual, expecting);           \
    }                                                                    \
    return PARSE_OK(S, T)(state, r.value);                               \
  }                                                                      \
  END_OF_STATEMENTS
