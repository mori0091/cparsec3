/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"

#define cparsec_module(M) CPARSEC_MODULE(M)()

#define CPARSEC_MODULE(M) TYPE_NAME(CparsecModule, M)
#define declare_Module(M) M CPARSEC_MODULE(M)(void)
