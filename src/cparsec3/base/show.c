/* -*- coding: utf-8-unix -*- */

#include <inttypes.h>

#include <cparsec3/base/base.h>

// -----------------------------------------------------------------------
#define TOSTRING(T) FUNC_NAME(toString, Show(T))
#define SHOW(T) FUNC_NAME(show, Show(T))

// -----------------------------------------------------------------------
impl_Show(int, "%d");
impl_Show(size_t, "%zu");

impl_Show(String, "\"%s\"");

impl_Show(int8_t, "%" PRId8);
impl_Show(int16_t, "%" PRId16);
impl_Show(int32_t, "%" PRId32);
impl_Show(int64_t, "%" PRId64);

impl_Show(uint8_t, "%" PRIu8);
impl_Show(uint16_t, "%" PRIu16);
impl_Show(uint32_t, "%" PRIu32);
impl_Show(uint64_t, "%" PRIu64);

// -----------------------------------------------------------------------
static inline void TOSTRING(None)(CharBuff* b, None x) {
  UNUSED(x);
  assert(0 <= mem_printf(b, "%s", "NONE"));
}

instance_Show(None, TOSTRING(None));

// -----------------------------------------------------------------------
#include <ctype.h>

void quote_char(CharBuff* b, char x) {
  switch (x) {
  case '\'':
    assert(0 <= mem_printf(b, "%s", "\\\'"));
    return;
  case '\"':
    assert(0 <= mem_printf(b, "%s", "\\\""));
    return;
  case '\\':
    assert(0 <= mem_printf(b, "%s", "\\\\"));
    return;
  case '\a':
    assert(0 <= mem_printf(b, "%s", "\\a"));
    return;
  case '\b':
    assert(0 <= mem_printf(b, "%s", "\\b"));
    return;
  case '\f':
    assert(0 <= mem_printf(b, "%s", "\\f"));
    return;
  case '\n':
    assert(0 <= mem_printf(b, "%s", "\\n"));
    return;
  case '\r':
    assert(0 <= mem_printf(b, "%s", "\\r"));
    return;
  case '\t':
    assert(0 <= mem_printf(b, "%s", "\\t"));
    return;
  case '\v':
    assert(0 <= mem_printf(b, "%s", "\\v"));
    return;
  default:
    if (iscntrl(x) || !isprint(x)) {
      assert(0 <= mem_printf(b, "\\%o", (int)(uint8_t)x));
    } else {
      assert(0 <= mem_printf(b, "%c", x));
    }
    return;
  }
}

static inline void TOSTRING(char)(CharBuff* b, char x) {
  mem_printf(b, "'");
  quote_char(b, x);
  mem_printf(b, "'");
}

static inline String SHOW(char)(char x) {
  CharBuff b = {0};
  TOSTRING(char)(&b, x);
  return b.data;
}

Show(char) Trait(Show(char)) {
  return (Show(char)){
      .toString = TOSTRING(char),
      .show = SHOW(char),
  };
}

// -----------------------------------------------------------------------
static inline void TOSTRING(bool)(CharBuff* b, bool x) {
  assert(0 <= mem_printf(b, "%s", (x ? "true" : "false")));
}

instance_Show(bool, TOSTRING(bool));

// -----------------------------------------------------------------------
impl_ShowSeq(Array, char);
impl_ShowSeq(List, char);
impl_ShowSeq(Array, String);
impl_ShowSeq(List, String);
