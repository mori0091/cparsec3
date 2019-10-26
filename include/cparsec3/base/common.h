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
