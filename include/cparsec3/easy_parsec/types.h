/* -*- coding: utf-8-unix -*- */
#pragma once

#define PARSER_RETURN_TYPES(S)                                           \
  None, PARSER_RETURN_TYPES_0(S), PARSER_RETURN_TYPES_1(S)
#define PARSER_RETURN_TYPES_0(S) Token(S), Tokens(S)
#define PARSER_RETURN_TYPES_1(S) APPLY(Array, PARSER_RETURN_TYPES_0(S))
