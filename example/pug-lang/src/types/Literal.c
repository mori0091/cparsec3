/* -*- coding: utf-8-unix -*- */

#include "types/Literal.h"

impl_user_type(Literal);

show_user_type(Literal)(CharBuff* b, Literal x) {
  switch (x.id) {
  case LIT_INTEGER:
    mem_printf(b, "%" PRId64, x.num.value);
    break;
  case LIT_UNIT:
    mem_printf(b, "()");
    break;
  case LIT_TRUE:
    mem_printf(b, "true");
    break;
  case LIT_FALSE:
    mem_printf(b, "false");
    break;
  default:
    assert(0 && "Illegal Literal");
    break;
  }
}
