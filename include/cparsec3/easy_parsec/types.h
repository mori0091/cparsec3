/* -*- coding: utf-8-unix -*- */
#pragma once

#define PARSER_RETURN_TYPES(S)                                           \
  None, PARSER_RETURN_TYPES_0(S), APPLY(Array, PARSER_RETURN_TYPES_0(S))

#define PARSER_RETURN_TYPES_0(S)                                         \
  SQUASH(Token(S), Tokens(S), CPARSEC_USER_TYPES())

#ifndef CPARSEC_CONFIG_USER_TYPES
#define CPARSEC_USER_TYPES()
#else
#define CPARSEC_USER_TYPES() CPARSEC_CONFIG_USER_TYPES
#endif
