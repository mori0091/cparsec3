/* -*- coding: utf-8-unix -*- */
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metac2.h"

#define UNUSED(x) ((void)x)

#define END_OF_STATEMENTS static_assert(1, "end of statements")

/**
 * \brief Constructs a type-name identifier.
 */
#define TYPE_NAME(...) MANGLE(__VA_ARGS__)

/**
 * \brief Constructs a function-name identifier.
 */
#define FUNC_NAME(x, ...) CAT(f_, CAT(x, CAT(_, MANGLE(__VA_ARGS__))))

/**
 * \brief Constructs an identifier.
 *
 * ~~~c
 * MANGLE(A)       // -> A
 * MANGLE(A, B)    // -> pA_B
 * MANGLE(A, B, C) // -> ppA_B_C
 * ~~~
 */
#define MANGLE(...)                                                      \
  IF(IS_NIL(__VA_ARGS__))                                                \
  (HEAD(__VA_ARGS__), FOLDL(MANGLE_PAIR, __VA_ARGS__))
#define MANGLE_PAIR(x, y) CAT(p, CAT(x, CAT(_, y)))

/**
 * \brief F(T); ... for each T in variadic arguments.
 *
 * ~~~c
 * FOREACH(F, int, char, String);
 * // -> F(int); F(char); F(String);
 *
 * #define F(T) T CAT(T, _value)
 * FOREACH(F, int, char, String);
 * // -> int int_value; char char_value; String String_value;
 * ~~~
 */
#define FOREACH(F, ...) SEP_BY(SEMICOLON, F, __VA_ARGS__)

/**
 * \brief Filters out dismissed items in the list.
 *
 * ~~~c
 * #define DISMISS_String JUST(1)
 * FILTER_OUT_DISMISSED(char, int, String, double)
 * // -> char, int, double
 * #undef DISMISS_String
 * FILTER_OUT_DISMISSED(char, int, String, double)
 * // -> char, int, String, double
 * ~~~
 */
#define FILTER_OUT_DISMISSED(...)                                        \
  FILTER(EXCLUDE_IF_DISMISSED, __VA_ARGS__)
#define FILTER(F, ...) SQUASH(APPLY(F, __VA_ARGS__))
#define EXCLUDE_IF_DISMISSED(x) IF(DISMISSED(x))(, x)
#define DISMISSED(x) EXTRACT_OR_DEFAULT(CAT(DISMISS_, x), 0)

#ifdef __cplusplus
#define C_API_BEGIN extern "C" {
#define C_API_END }
#else
#define C_API_BEGIN
#define C_API_END
#endif

C_API_BEGIN

typedef const char* String;
typedef int Int;
typedef intmax_t Offset;

C_API_END
