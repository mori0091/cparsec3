/* -*- coding: utf-8-unix -*- */

#include "types/Pat.h"

impl_List(Pat);

static Pat Pat_New(void) {
  Pat p = (Pat)mem_malloc(sizeof(struct Pat));
  return p;
}

static Pat FUNC_NAME(PWildcard, Pat)(void) {
  static struct Pat p = {.id = PWILDCARD};
  return &p;
}

static Pat FUNC_NAME(PVar, Pat)(Id ident) {
  Pat p = Pat_New();
  p->id = PVAR;
  p->ident = ident;
  return p;
}

static Pat FUNC_NAME(PLit, Pat)(Literal lit) {
  Pat p = Pat_New();
  p->id = PLITERAL;
  p->literal = lit;
  return p;
}

static Pat FUNC_NAME(PCon, Pat)(Id ident) {
  Pat p = Pat_New();
  p->id = PCON;
  p->ident = ident;
  return p;
}

static Pat FUNC_NAME(PCAp, Pat)(Pat lhs, Pat rhs) {
  Pat p = Pat_New();
  p->id = PCAPPLY;
  p->lhs = lhs;
  p->rhs = rhs;
  return p;
}

PatT Trait(Pat) {
  return (PatT){
      .PWildcard = FUNC_NAME(PWildcard, Pat),
      .PVar = FUNC_NAME(PVar, Pat),
      .PLit = FUNC_NAME(PLit, Pat),
      .PCon = FUNC_NAME(PCon, Pat),
      .PCAp = FUNC_NAME(PCAp, Pat),
  };
}
