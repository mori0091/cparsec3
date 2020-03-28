/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base_generics.h"

#include "config.h"
#include "types.h"
#include "def_parsec.h"

#include "../parsec/parsec.h"
#include "../parsec/stream.h"

#include "../parsec/ParsecRunner.h"

// -----------------------------------------------------------------------
// ParsecRunner(S, T)

#define TRAIT_PARSECRUNNER(S, T) trait(ParsecRunner(S, T))

#define GENERIC_PARSECRUNNER(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECRUNNER, BIND(S, PARSER_RETURN_TYPES(S)))

#define runParsec(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p).pRunParsec(p, input)

#define runParser(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p).pRunParser(p, input)

#define parseTest(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p)                           \
      .pParseTest(p, CPARSEC_STREAM_NEW(input))

// -----------------------------------------------------------------------
trait_ParseError(CPARSEC_STREAM_TYPE);
BIND_FOR(trait_ParsecRunner, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));

#ifdef CPARSEC_CONFIG_IMPLEMENT
impl_ParseError(CPARSEC_STREAM_TYPE);
BIND_FOR(impl_ParsecRunner, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));
#endif
