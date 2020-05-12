/* -*- coding: utf-8-unix -*- */

#include "types/Kind.h"

impl_user_type(Kind);

Kind FUNC_NAME(Star, Kind)(void) {
  static struct Kind star = {.id = STAR};
  return &star;
}

Kind FUNC_NAME(Kfun, Kind)(Kind k1, Kind k2) {
  Kind k = mem_malloc(sizeof(struct Kind));
  k->id = KFUN;
  k->lhs = k1;
  k->rhs = k2;
  return k;
}

KindT Trait(Kind) {
  return (KindT){
      .Star = FUNC_NAME(Star, Kind),
      .Kfun = FUNC_NAME(Kfun, Kind),
  };
}

show_user_type(Kind)(CharBuff* b, Kind x) {
  switch (x->id) {
  case STAR:
    mem_printf(b, "*");
    break;
  case KFUN:;
    Show(Kind) S = trait(Show(Kind));
    mem_printf(b, "(");
    S.toString(b, x->lhs);
    mem_printf(b, "->");
    S.toString(b, x->rhs);
    mem_printf(b, ")");
    break;
  default:
    break;
  }
}
