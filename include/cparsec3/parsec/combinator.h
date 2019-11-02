/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../core/core.h"

// -----------------------------------------------------------------------
/**
 * \brief Name of parser constructor function.
 *
 * ~~~c
 * Parsec(S, T) p = parsec(S, T, foo)('x', 3);
 * ~~~
 */
#define parsec(S, T, name) FUNC_NAME(name, S, T)

/**
 * \brief Declares a parser.
 *
 * \param S    type of stream
 * \param T    type of parser's return value
 * \param ...  name, and types of parameters
 *
 * ~~~c
 * // Parsec(S, T) foo(char, int)
 * declare_parsec(S, T, foo, char, int);
 * ~~~
 *
 */
#define declare_parsec(S, T, ...)                                        \
  IF(IS_NIL(__VA_ARGS__, ))                                              \
  (declare_parser, declare_combinator)(S, T, __VA_ARGS__)

/**
 * \brief Defines a parser.
 *
 * \param S    type of stream
 * \param T    type of parser's return value
 * \param ...  name, and types of parameters
 *
 * ~~~c
 * // Parsec(S, T) foo(char, int)
 * define_parsec(S, T, foo, char, int) {
 *   char arg1 = arg->_1;
 *   int arg2 = arg->_2;
 *   ...
 *   T res = ...;
 *   return PARSE_OK(S, T)(state, res);
 * }
 * ~~~
 *
 */
#define define_parsec(S, T, ...)                                         \
  IF(IS_NIL(__VA_ARGS__, ))                                              \
  (define_parser, define_combinator)(S, T, __VA_ARGS__)


// -----------------------------------------------------------------------
#define PARAMETERS(...)                                                  \
  CAT(PARAMETERS, VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)
#define PARAMETERS1(T1) T1 _1
#define PARAMETERS2(T1, T2) T1 _1, T2 _2
#define PARAMETERS3(T1, T2, T3) T1 _1, T2 _2, T3 _3

// -----------------------------------------------------------------------
#define ARGUMENTS(...) CAT(ARGUMENTS, VARIADIC_SIZE(__VA_ARGS__))()
#define ARGUMENTS1() _1
#define ARGUMENTS2() _1, _2
#define ARGUMENTS3() _1, _2, _3

// -----------------------------------------------------------------------
#define struct_args(tag, ...)                                            \
  CAT(struct_args, VARIADIC_SIZE(__VA_ARGS__))(tag, __VA_ARGS__)
#define struct_args1(tag, T1)                                            \
  struct tag {                                                           \
    T1 _1;                                                               \
  }
#define struct_args2(tag, T1, T2)                                        \
  struct tag {                                                           \
    T1 _1;                                                               \
    T2 _2;                                                               \
  }
#define struct_args3(tag, T1, T2, T3)                                    \
  struct tag {                                                           \
    T1 _1;                                                               \
    T2 _2;                                                               \
    T3 _3;                                                               \
  }

// -----------------------------------------------------------------------
#define declare_parser(S, T, name)                                       \
  Parsec(S, T) FUNC_NAME(name, S, T)(void)

#define define_parser(S, T, name)                                        \
  Parsec(S, T) FUNC_NAME(name, S, T)(void);                              \
  static ParseStateResult(S, T)                                          \
      FUNC_NAME(name##_run, S, T)(void*, ParseState(S));                 \
  static inline ParseStateResult(S, T)                                   \
      FUNC_NAME(name##_i, S, T)(ParseState(S));                          \
  /* constructor of parser, or parser combinator */                      \
  Parsec(S, T) FUNC_NAME(name, S, T)(void) {                             \
    return (Parsec(S, T)){.run = FUNC_NAME(name##_run, S, T),            \
                          .arg = NULL};                                  \
  }                                                                      \
  /* parser body */                                                      \
  static ParseStateResult(S, T)                                          \
      FUNC_NAME(name##_run, S, T)(void* arg, ParseState(S) state) {      \
    UNUSED(arg);                                                         \
    return FUNC_NAME(name##_i, S, T)(state);                             \
  }                                                                      \
  /* implementation of parser body */                                    \
  static inline ParseStateResult(S, T)                                   \
      FUNC_NAME(name##_i, S, T)(ParseState(S) state)

// -----------------------------------------------------------------------
#define declare_combinator(S, T, name, ...)                              \
  Parsec(S, T) FUNC_NAME(name, S, T)(__VA_ARGS__)

#define define_combinator(S, T, name, ...)                               \
  Parsec(S, T) FUNC_NAME(name, S, T)(__VA_ARGS__);                       \
  struct_args(TYPE_NAME(name, S, T), __VA_ARGS__);                       \
  static ParseStateResult(S, T)                                          \
      FUNC_NAME(name##_run, S, T)(void*, ParseState(S));                 \
  static inline ParseStateResult(S, T) FUNC_NAME(name##_i, S, T)(        \
      struct TYPE_NAME(name, S, T)*, ParseState(S));                     \
  /* constructor of parser, or parser combinator */                      \
  Parsec(S, T) FUNC_NAME(name, S, T)(PARAMETERS(__VA_ARGS__)) {          \
    struct TYPE_NAME(name, S, T)* arg = malloc(sizeof(arg[0]));          \
    *arg = (struct TYPE_NAME(name, S, T)){ARGUMENTS(__VA_ARGS__)};       \
    return (Parsec(S, T)){.run = FUNC_NAME(name##_run, S, T),            \
                          .arg = arg};                                   \
  }                                                                      \
  /* parser body */                                                      \
  static ParseStateResult(S, T)                                          \
      FUNC_NAME(name##_run, S, T)(void* arg, ParseState(S) state) {      \
    return FUNC_NAME(name##_i, S, T)((struct TYPE_NAME(name, S, T)*)arg, \
                                     state);                             \
  }                                                                      \
  /* implementation of parser body */                                    \
  static inline ParseStateResult(S, T) FUNC_NAME(name##_i, S, T)(        \
      struct TYPE_NAME(name, S, T) * arg, ParseState(S) state)
