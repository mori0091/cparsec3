/* -*- coding: utf-8-unix -*- */

#define CPARSEC_CONFIG_DATA_SOURCE String
#define CPARSEC_CONFIG_POSITIONER

#include <cparsec3/easy_parsec/parser/char.h>
#include <cparsec3/easy_parsec/parser/choice.h>
#include <cparsec3/easy_parsec/parser/combinator.h>
#include <cparsec3/easy_parsec/parser/failure.h>
#include <cparsec3/easy_parsec/parser/repeat.h>
#include <cparsec3/easy_parsec/parser/token.h>

extern PARSER(String) abc(void);
