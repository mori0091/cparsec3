/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

/* for backward compatibility */
#define cparsec_module(M) CPARSEC_MODULE(M)()
#define CPARSEC_MODULE(M) FUNC_NAME(Trait, M)
