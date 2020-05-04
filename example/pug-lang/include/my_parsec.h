/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// #define CPARSEC_CONFIG_IMPLEMENT /* generate parsec code if defined */

// -----------------------------------------------------------------------
#define CPARSEC_CONFIG_USER_TYPES Expr, Type
#include "types/Expr.h"

// -----------------------------------------------------------------------
// #define CPARSEC_CONFIG_DATA_SOURCE String
// #define CPARSEC_CONFIG_POSITIONER Text  /* line/column */
// #define CPARSEC_CONFIG_POSITIONER Index /* index */
// #define CPARSEC_CONFIG_POSITIONER       /* memory address or else */

#include <cparsec3/easy_parsec/parser/char.h>
#include <cparsec3/easy_parsec/parser/choice.h>
#include <cparsec3/easy_parsec/parser/combinator.h>
#include <cparsec3/easy_parsec/parser/failure.h>
#include <cparsec3/easy_parsec/parser/repeat.h>
#include <cparsec3/easy_parsec/parser/token.h>
