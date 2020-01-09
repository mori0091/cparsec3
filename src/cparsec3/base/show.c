/* -*- coding: utf-8-unix -*- */

#include <inttypes.h>

#include <cparsec3/base/base.h>

// -----------------------------------------------------------------------
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
static inline String SHOW(None)(None x) {
  UNUSED(x);
  char* s;
  assert(0 <= mem_asprintf(&s, "%s", "NONE"));
  return s;
}

instance_Show(None, SHOW(None));

// -----------------------------------------------------------------------
#include <ctype.h>

static inline String SHOW(char)(char x) {
  char* s = NULL;
  switch (x) {
  case '\'':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\\'"));
    return s;
  case '\"':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\\""));
    return s;
  case '\\':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\\\"));
    return s;
  case '\a':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\a"));
    return s;
  case '\b':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\b"));
    return s;
  case '\f':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\f"));
    return s;
  case '\n':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\n"));
    return s;
  case '\r':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\r"));
    return s;
  case '\t':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\t"));
    return s;
  case '\v':
    assert(0 <= mem_asprintf(&s, "'%s'", "\\v"));
    return s;
  default:
    break;
  }
  if (iscntrl(x) || !isprint(x)) {
    assert(0 <= mem_asprintf(&s, "'\\%o'", (int)(uint8_t)x));
    return s;
  }
  assert(0 <= mem_asprintf(&s, "'%c'", x));
  return s;
}

instance_Show(char, SHOW(char));

// -----------------------------------------------------------------------
static inline String SHOW(bool)(bool x) {
  char* s;
  assert(0 <= mem_asprintf(&s, "%s", (x ? "true" : "false")));
  return s;
}

instance_Show(bool, SHOW(bool));
