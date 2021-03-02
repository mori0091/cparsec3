/* -*- coding: utf-8-unix -*- */

#include "types/Literal.h"

#include "cparsec3/base/base_generics.h"
#include <ctype.h>

impl_user_type(Literal);

static void quote(CharBuff* b, Array(char) xs) {
  mem_printf(b, "\"");
  for (char* p = g_begin(xs); p != g_end(xs); p++) {
    switch (*p) {
    case '\\':
      mem_printf(b, "\\\\");
      break;
    case '\a':
      mem_printf(b, "\\a");
      break;
    case '\b':
      mem_printf(b, "\\b");
      break;
    case '\f':
      mem_printf(b, "\\f");
      break;
    case '\n':
      mem_printf(b, "\\n");
      break;
    case '\r':
      mem_printf(b, "\\r");
      break;
    case '\t':
      mem_printf(b, "\\t");
      break;
    case '\v':
      mem_printf(b, "\\v");
      break;
    case '\0':
      mem_printf(b, "\\0");
      break;
    case '\"':
      mem_printf(b, "\\\"");
      break;
    case '\'':
      mem_printf(b, "\\\'");
      break;
    case '\?':
      mem_printf(b, "\\\?");
      break;
    default:
      mem_printf(b, "%c", (isprint(*p) ? *p : '\?'));
      break;
    }
  }
  mem_printf(b, "\"");
}

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
  case LIT_STRING:
    quote(b, x.str);
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
    return g_eq(a.num.value, b.num.value);
  }
  if (a.id == LIT_STRING) {
    return g_eq(a.str, b.str);
  }
  return true;
}
instance_Eq(Literal, FUNC_NAME(eq, Eq(Literal)));

static bool FUNC_NAME(le, Ord(Literal))(Literal a, Literal b) {
  if (a.id == LIT_INTEGER && b.id == LIT_INTEGER) {
    return a.num.value <= b.num.value;
  }
  if (a.id == LIT_STRING && b.id == LIT_STRING) {
    return g_le(a.str, b.str);
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
