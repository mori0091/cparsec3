/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Id.h"
#include "Literal.h"

enum PatId {
  /* wildcard `_` */
  PWILDCARD,
  /* variable */
  PVAR,
  /* literal */
  PLITERAL,
  /* constructor */
  PCON,
  /* constructor application */
  PCAPPLY,
};

typedef struct Pat* Pat;
struct Pat {
  enum PatId id;
  union {
    struct {
      Pat lhs;
      Pat rhs;
    };
    Literal literal;
    Id ident; // for PVAR, PCON
  };
};

trait_List(Pat);

typedef struct PatT PatT;
struct PatT {
  Pat (*PWildcard)(void);
  Pat (*PVar)(Id ident);
  Pat (*PLit)(Literal lit);
  Pat (*PCon)(Id ident);
  Pat (*PCAp)(Pat lhs, Pat rhs);
};

PatT Trait(Pat);
