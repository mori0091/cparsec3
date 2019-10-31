/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define Trait(T) FUNC_NAME(Trait, T)(void)
#define trait(T) FUNC_NAME(Trait, T)()

#define Data(...) TYPE_NAME(Data, __VA_ARGS__)
