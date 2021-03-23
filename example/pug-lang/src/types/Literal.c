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

static bool FUNC_NAME(eq, Eq(Literal))(Literal a, Literal b) {
  if (a.id != b.id) {
    return false;
  }
  if (a.id == LIT_INTEGER) {
    return a.num.value == b.num.value;
  } else {
    return true;
  }
}
instance_Eq(Literal, FUNC_NAME(eq, Eq(Literal)));

static bool FUNC_NAME(le, Ord(Literal))(Literal a, Literal b) {
  if (a.id == LIT_INTEGER && b.id == LIT_INTEGER) {
    return a.num.value <= b.num.value;
  }
  if (a.id == b.id) {
    return true;
  }
  if (a.id == LIT_FALSE && b.id == LIT_TRUE) {
    return true;
  }
  if (a.id == LIT_TRUE && b.id == LIT_FALSE) {
    return false;
  }
  assert(0 && "Illegal Literal");
  return false;
}
instance_Ord(Literal, FUNC_NAME(le, Ord(Literal)),
             FUNC_NAME(eq, Eq(Literal)));
