/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

#define TYPESET(C) SQUASH(CAT(TYPESET_, C))

#define TYPESET_PRIMITIVE None, char, String, Int
#define TYPESET_ALL TYPESET_PRIMITIVE,
