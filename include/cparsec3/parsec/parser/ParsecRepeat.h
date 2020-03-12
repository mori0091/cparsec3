/* -*- coding: utf-8-unix -*- */
#pragma once

#include "ParsecDeriv.h"

// -----------------------------------------------------------------------
#define ParsecRepeat(...) TYPE_NAME(ParsecRepeat, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecRepeat(S, T)                                         \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ParsecRepeat(S, T) ParsecRepeat(S, T);                  \
  struct ParsecRepeat(S, T) {                                            \
    Parsec(S, Array(T)) (*many)(Parsec(S, T) p);                         \
    Parsec(S, Array(T)) (*some)(Parsec(S, T) p);                         \
    Parsec(S, Array(T)) (*count)(int n, Parsec(S, T) p);                 \
    Parsec(S, Array(T)) (*count_min_max)(int m, int n, Parsec(S, T) p);  \
  };                                                                     \
                                                                         \
  ParsecRepeat(S, T) Trait(ParsecRepeat(S, T));                          \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecRepeat(S, T)                                          \
  C_API_BEGIN                                                            \
                                                                         \
  typedef_Fn(Parsec(S, T), UnParser(S, Array(T)));                       \
                                                                         \
  impl_many(S, T);                                                       \
  impl_some(S, T);                                                       \
  impl_count(S, T);                                                      \
  impl_count_min_max(S, T);                                              \
                                                                         \
  ParsecRepeat(S, T) Trait(ParsecRepeat(S, T)) {                         \
    return (ParsecRepeat(S, T)){                                         \
        .many = FUNC_NAME(many, S, T),                                   \
        .some = FUNC_NAME(some, S, T),                                   \
        .count = FUNC_NAME(count, S, T),                                 \
        .count_min_max = FUNC_NAME(count_min_max, S, T),                 \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_many(S, T)                                                  \
  fn(FUNC_NAME(many_fn, S, T), Parsec(S, T),                             \
     UnParserArgs(S, Array(T))) {                                        \
    g_bind((p, s, cok, cerr, eok), *args);                               \
    ParsecRunner(S, T) R = trait(ParsecRunner(S, T));                    \
    ArrayT(T) A = trait(Array(T));                                       \
    ParseReply(S, T) r = R.runParsec(p, s);                              \
    if (!r.consumed) {                                                   \
      return fn_apply(eok, A.empty, s, (Hints(Token(S))){0});            \
    }                                                                    \
    if (!r.result.success) {                                             \
      return fn_apply(cerr, r.result.err, r.result.state);               \
    }                                                                    \
    Array(T) a = A.create(1);                                            \
    a.data[0] = r.result.ok;                                             \
    for (;;) {                                                           \
      s = r.result.state;                                                \
      r = R.runParsec(p, s);                                             \
      if (!r.consumed) {                                                 \
        return fn_apply(cok, a, r.result.state, (Hints(Token(S))){0});   \
      }                                                                  \
      if (!r.result.success) {                                           \
        return fn_apply(cerr, r.result.err, r.result.state);             \
      }                                                                  \
      a.data = trait(Mem(T)).recreate(a.data, a.length + 1);             \
      a.data[a.length] = r.result.ok;                                    \
      a.length++;                                                        \
    }                                                                    \
  }                                                                      \
                                                                         \
  static Parsec(S, Array(T)) FUNC_NAME(many, S, T)(Parsec(S, T) p) {     \
    __auto_type f = FUNC_NAME(many_fn, S, T)();                          \
    return (Parsec(S, Array(T))){.unParser = fn_apply(f, p)};            \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_some(S, T)                                                  \
  static Parsec(S, Array(T)) FUNC_NAME(some, S, T)(Parsec(S, T) p) {     \
    return (Parsec(S, Array(T))){0};                                     \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_count(S, T)                                                 \
  static Parsec(S, Array(T))                                             \
      FUNC_NAME(count, S, T)(int n, Parsec(S, T) p) {                    \
    return (Parsec(S, Array(T))){0};                                     \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_count_min_max(S, T)                                         \
  static Parsec(S, Array(T))                                             \
      FUNC_NAME(count_min_max, S, T)(int m, int n, Parsec(S, T) p) {     \
    return (Parsec(S, Array(T))){0};                                     \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
