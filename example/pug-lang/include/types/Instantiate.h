/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Pred.h"
#include "Qual.h"
#include "Type.h"

#define Instantiate(T) TYPE_NAME(Instantiate, T)

#define trait_Instantiate(T)                                             \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct Instantiate(T) Instantiate(T);                          \
  struct Instantiate(T) {                                                \
    T (*inst)(List(Type) ts, T t);                                       \
  };                                                                     \
                                                                         \
  Instantiate(T) Trait(Instantiate(T));                                  \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

trait_Instantiate(Type);
trait_Instantiate(List(Type));

trait_Instantiate(Qual(Type));
trait_Instantiate(Qual(List(Type)));
// trait_Instantiate(List(Qual(Type)));
// trait_Instantiate(List(Qual(List(Type))));

trait_Instantiate(Pred);
trait_Instantiate(List(Pred));
