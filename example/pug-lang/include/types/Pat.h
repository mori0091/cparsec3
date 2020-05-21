/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#include "Assump.h"
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
};

typedef struct Pat* Pat;
decl_user_type(Pat);
trait_List(Pat);

struct Pat {
  enum PatId id;
  union {
    Id ident;         // for PVAR
    Literal literal;  // for PLit
    struct {          // for PCon
      Assump a;       // identifier and type-scheme of constructor
      List(Pat) pats; // argument patterns of constructor
    };
  };
};

typedef struct PatT PatT;
struct PatT {
  Pat (*PWildcard)(void);
  Pat (*PVar)(Id ident);
  Pat (*PLit)(Literal lit);
  Pat (*PCon)(Assump a, List(Pat) pats);
};

PatT Trait(Pat);
