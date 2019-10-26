/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "module.h"

#define Eq(T) CONCAT(Eq, T)
#define declare_Eq(T)                                                    \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    bool (*eq)(T, T);                                                    \
    bool (*neq)(T, T);                                                   \
  } Eq(T);                                                               \
  Eq(T) CPARSEC_MODULE(Eq(T))(void);                                     \
  C_API_END                                                              \
  END_OF_STATEMENTS

declare_Eq(char);
declare_Eq(int);

declare_Eq(int8_t);
declare_Eq(int16_t);
declare_Eq(int32_t);
declare_Eq(int64_t);

declare_Eq(uint8_t);
declare_Eq(uint16_t);
declare_Eq(uint32_t);
declare_Eq(uint64_t);

declare_Eq(String);
