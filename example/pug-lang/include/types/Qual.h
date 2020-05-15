/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Pred.h"

#include "Assump.h"
#include "Scheme.h"
#include "Type.h"

#define Qual(T) TYPE_NAME(Qual, T)
#define QualT(T) TYPE_NAME(QualT, T)

#define trait_Qual(T)                                                    \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct Qual(T) Qual(T);                                        \
  struct Qual(T) {                                                       \
    List(Pred) ps;                                                       \
    T t;                                                                 \
  };                                                                     \
                                                                         \
  typedef struct QualT(T) QualT(T);                                      \
  struct QualT(T) {                                                      \
    Qual(T) (*create)(List(Pred) ps, T t);                               \
  };                                                                     \
                                                                         \
  trait_Eq(Qual(T));                                                     \
                                                                         \
  QualT(T) Trait(Qual(T));                                               \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

trait_Qual(Type);
trait_Qual(Scheme);
trait_Qual(Assump);

trait_Qual(List(Type));
trait_Qual(List(Scheme));
trait_Qual(List(Assump));

trait_Types(Qual(Type));
trait_Types(Qual(Scheme));
trait_Types(Qual(Assump));

trait_Types(Qual(List(Type)));
trait_Types(Qual(List(Scheme)));
trait_Types(Qual(List(Assump)));
