/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "module.h"

#define Ord(T) CONCAT(Ord, T)
#define declare_Ord(T)                                                   \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    union {                                                              \
      int (*cmp)(T, T);                                                  \
      int (*compare)(T, T);                                              \
    };                                                                   \
    union {                                                              \
      bool (*lt)(T, T);                                                  \
      bool (*lessThan)(T, T);                                            \
    };                                                                   \
    union {                                                              \
      bool (*le)(T, T);                                                  \
      bool (*lessThanEqual)(T, T);                                       \
    };                                                                   \
    union {                                                              \
      bool (*gt)(T, T);                                                  \
      bool (*greaterThan)(T, T);                                         \
    };                                                                   \
    union {                                                              \
      bool (*ge)(T, T);                                                  \
      bool (*greaterThanEqual)(T, T);                                    \
    };                                                                   \
    T (*min)(T, T);                                                      \
    T (*max)(T, T);                                                      \
  } Ord(T);                                                              \
  Ord(T) CPARSEC_MODULE(Ord(T))(void);                                   \
  C_API_END                                                              \
  END_OF_STATEMENTS

declare_Ord(char);
declare_Ord(int);

declare_Ord(int8_t);
declare_Ord(int16_t);
declare_Ord(int32_t);
declare_Ord(int64_t);

declare_Ord(uint8_t);
declare_Ord(uint16_t);
declare_Ord(uint32_t);
declare_Ord(uint64_t);

declare_Ord(String);
