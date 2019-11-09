/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../core/core.h"
#include "combinator.h"

// -----------------------------------------------------------------------
enum eGuard { ANY, UNARY, BINARY };
#define testGuard(T) FUNC_NAME(testGuard, T)
#define Guard(T) TYPE_NAME(Guard, T)
#define typedef_Guard(T)                                                 \
  typedef struct {                                                       \
    enum eGuard type;                                                    \
    union {                                                              \
      struct {                                                           \
        bool (*op1)(T);                                                  \
      };                                                                 \
      struct {                                                           \
        bool (*op2)(T, T);                                               \
        T lhs;                                                           \
      };                                                                 \
    };                                                                   \
  } Guard(T);                                                            \
  bool testGuard(T)(Guard(T) g, T x)

#define define_Guard(T)                                                  \
  bool testGuard(T)(Guard(T) g, T x) {                                   \
    switch (g.type) {                                                    \
    case ANY:                                                            \
      return true;                                                       \
    case UNARY:                                                          \
      return g.op1(x);                                                   \
    case BINARY:                                                         \
      return g.op2(g.lhs, x);                                            \
    }                                                                    \
    fprintf(stderr, "Malformed Guard(T) object");                        \
    abort();                                                             \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define applyTrans(T, R) FUNC_NAME(applyTrans, T, R)
#define Trans(T, R) TYPE_NAME(Trans, T, R)
#define typedef_Trans(T, R)                                              \
  typedef struct {                                                       \
    bool constant;                                                       \
    union {                                                              \
      R value;                                                           \
      R (*transmute)(T);                                                 \
    };                                                                   \
  } Trans(T, R);                                                         \
  R applyTrans(T, R)(Trans(T, R) t, T x)

#define define_Trans(T, R)                                               \
  R applyTrans(T, R)(Trans(T, R) t, T x) {                               \
    if (t.constant) {                                                    \
      return t.value;                                                    \
    }                                                                    \
    return t.transmute(x);                                               \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define testMatch(T, R) FUNC_NAME(testMatch, T, R)
#define Match(T, R) TYPE_NAME(Match, T, R)
#define typedef_Match(T, R)                                              \
  /* typedef_Guard(T); */                                                \
  /* typedef_Trans(T, R); */                                             \
  typedef struct {                                                       \
    Guard(T) guard;                                                      \
    Trans(T, R) trans;                                                   \
  } Match(T, R);                                                         \
  Maybe(R) testMatch(T, R)(Match(T, R) m, T x)

#define define_Match(T, R)                                               \
  Maybe(R) testMatch(T, R)(Match(T, R) m, T x) {                         \
    if (!testGuard(T)(m.guard, x)) {                                     \
      return (Maybe(R)){.none = true};                                   \
    }                                                                    \
    return (Maybe(R)){.value = applyTrans(T, R)(m.trans, x)};            \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define declare_token(S, T)                                              \
  /* typedef_Maybe(T); */                                                \
  /* typedef_Match(Token(S), T); */                                      \
  declare_parsec(S, T, token, Match(Token(S), T), Hints(S))

#define define_token(S, T)                                               \
  define_parsec(S, T, token, Match(Token(S), T), Hints(S)) {             \
    Match(Token(S), T) match = arg->_1;                                  \
    Hints(S) expecting = arg->_2;                                        \
    Stream(S) stream = trait(Stream(S));                                 \
    Maybe(Tuple(Token(S), S)) r0 = stream.take1(state.input);            \
    if (r0.none) {                                                       \
      return UNEXPECTED_EOM(S, T)(state, expecting);                     \
    }                                                                    \
    state.input = r0.value.second;                                       \
    state.offset++;                                                      \
    Token(S) actual = r0.value.first;                                    \
    Maybe(T) r = testMatch(Token(S), T)(match, actual);                  \
    if (r.none) {                                                        \
      return UNEXPECTED_TOKEN(S, T)(state, actual, expecting);           \
    }                                                                    \
    return PARSE_OK(S, T)(state, r.value);                               \
  }                                                                      \
  END_OF_STATEMENTS
