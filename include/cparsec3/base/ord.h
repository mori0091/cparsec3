/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "module.h"
#include "typeset.h"

#define Ord(T) TYPE_NAME(Ord, T)
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

FOREACH(declare_Ord, TYPESET(ALL));
