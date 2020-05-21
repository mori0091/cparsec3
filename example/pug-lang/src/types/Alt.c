/* -*- coding: utf-8-unix -*- */

#include "types/Alt.h"
#include "types/Expr.h"

impl_user_type(Alt);

show_user_type(Alt)(CharBuff* b, Alt x) {
  Show(Pat) S = trait(Show(Pat));
  mem_printf(b, "(");
  for (List(Pat) ps = x.pats; ps; ps = ps->tail) {
    S.toString(b, ps->head);
    mem_printf(b, " ");
  }
  mem_printf(b, "=> ");
  trait(Show(Expr)).toString(b, x.e);
  mem_printf(b, ")");
}

impl_List(Alt);
impl_user_type(List(Alt));
show_user_type(List(Alt))(CharBuff* b, List(Alt) as) {
  Show(Alt) S = trait(Show(Alt));
  if (!as) {
    mem_printf(b, "[]");
    return;
  }
  mem_printf(b, "[");
  S.toString(b, as->head);
  as = as->tail;
  while (as) {
    mem_printf(b, ", ");
    S.toString(b, as->head);
    as = as->tail;
  }
  mem_printf(b, "]");
}
