/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

typedef struct Kind* Kind;
decl_user_type(Kind);

trait_Eq(Kind);

trait_List(Kind);
trait_Eq(List(Kind));

enum KindId {
  STAR, // *
  KFUN, // k1 -> k2
};

struct Kind {
  enum KindId id;
  // only for KFUN
  struct {
    Kind lhs;
    Kind rhs;
  };
};

typedef struct KindT {
  Kind (*Star)(void);             // *
  Kind (*Star_Star)(void);        // * -> *
  Kind (*Star_Star_Star)(void);   // * -> * -> *
  Kind (*Kfun)(Kind k1, Kind k2); // k1 -> k2
} KindT;

KindT Trait(Kind);
