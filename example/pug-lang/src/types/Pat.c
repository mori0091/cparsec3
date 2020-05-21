/* -*- coding: utf-8-unix -*- */

#include "types/Pat.h"

impl_user_type(Pat);

show_user_type(Pat)(CharBuff* b, Pat pat) {
  Show(Pat) S = trait(Show(Pat));
  switch (pat->id) {
  case PWILDCARD:
    mem_printf(b, "_");
    break;
  case PVAR:
    mem_printf(b, "(PVar %s)", pat->ident);
    break;
  case PLITERAL:
    trait(Show(Literal)).toString(b, pat->literal);
    break;
  case PCON:
    mem_printf(b, "(%s", pat->a.ident);
    for (List(Pat) pats = pat->pats; pats; pats = pats->tail) {
      mem_printf(b, " ");
      S.toString(b, pats->head);
    }
    mem_printf(b, ")");
    break;
  default:
    assert(0 && "Illegal Pat");
    break;
  }
}

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

static Pat FUNC_NAME(PCon, Pat)(Assump a, List(Pat) pats) {
  Pat p = Pat_New();
  p->id = PCON;
  p->a = a;
  p->pats = pats;
  return p;
}

PatT Trait(Pat) {
  return (PatT){
      .PWildcard = FUNC_NAME(PWildcard, Pat),
      .PVar = FUNC_NAME(PVar, Pat),
      .PLit = FUNC_NAME(PLit, Pat),
      .PCon = FUNC_NAME(PCon, Pat),
  };
}
