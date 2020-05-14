/* -*- coding: utf-8-unix -*- */

#include "types/Kind.h"

impl_user_type(Kind);

static bool FUNC_NAME(eq, Eq(Kind))(Kind k1, Kind k2) {
  assert(k1 && "Null pointer");
  assert(k2 && "Null pointer");
  if (k1 == k2) {
    return true;
  }
  if (k1->id != k2->id) {
    return false;
  }
  if (k1->id == STAR) {
    return true;
  }
  return FUNC_NAME(eq, Eq(Kind))(k1->lhs, k2->lhs) &&
         FUNC_NAME(eq, Eq(Kind))(k1->rhs, k2->rhs);
}
instance_Eq(Kind, FUNC_NAME(eq, Eq(Kind)));

static struct Kind star = {
    .id = STAR,
    .lhs = 0,
    .rhs = 0,
};
static struct Kind star_star = {
    .id = KFUN,
    .lhs = &star,
    .rhs = &star,
};
static struct Kind star_star_star = {
    .id = KFUN,
    .lhs = &star,
    .rhs = &star_star,
};

Kind FUNC_NAME(Star, Kind)(void) {
  return &star;
}

Kind FUNC_NAME(Star_Star, Kind)(void) {
  return &star_star;
}

Kind FUNC_NAME(Star_Star_Star, Kind)(void) {
  return &star_star_star;
}

Kind FUNC_NAME(Kfun, Kind)(Kind k1, Kind k2) {
  assert(k1 && k2 && "Null pointer");
  Kind k = mem_malloc(sizeof(struct Kind));
  k->id = KFUN;
  k->lhs = k1;
  k->rhs = k2;
  return k;
}

KindT Trait(Kind) {
  return (KindT){
      .Star = FUNC_NAME(Star, Kind),
      .Star_Star = FUNC_NAME(Star_Star, Kind),
      .Star_Star_Star = FUNC_NAME(Star_Star_Star, Kind),
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
