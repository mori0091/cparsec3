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

#define CONCAT(...) CONCAT0(__VA_ARGS__, )
#define CONCAT0(x, ...)                                                  \
  IF(IS_NIL(__VA_ARGS__))(x, FOLDL(CONCAT1, SQUASH(x, __VA_ARGS__)))
#define CONCAT1(x, y) CAT(x, CAT(_, y))

// ---- F(T); ... for each T in varargs
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
