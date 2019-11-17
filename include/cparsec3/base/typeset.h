/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "none.h"

#define TYPESET(C) SQUASH(CAT(TYPESET_, C))

#define TYPESET_STD_INT                                                  \
  int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t

#define TYPESET_PRIMITIVE None, char, String, TYPESET_STD_INT

#define TYPESET_ALL int, TYPESET_PRIMITIVE
