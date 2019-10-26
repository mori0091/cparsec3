/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define cparsec_module(M) CPARSEC_MODULE(M)()

#define CPARSEC_MODULE(M) CONCAT(cparsec_module, M)
#define declare_Module(M) M CPARSEC_MODULE(M)(void)
