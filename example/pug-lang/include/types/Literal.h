/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

/** number (integer) */
typedef struct Num {
  int64_t value;
} Num;

enum LiteralId {
  /* integer */
  LIT_INTEGER,
  /* () (singleton value of unit type)*/
  LIT_UNIT,
  /* `true` */
  LIT_TRUE,
  /* `false` */
  LIT_FALSE,
  /* string */
  LIT_STRING,
};

typedef struct Literal Literal;
struct Literal {
  enum LiteralId id;
  union {
    Num num;
    Array(char) str;
  };
};

decl_user_type(Literal);
trait_Eq(Literal);
trait_Ord(Literal);

static inline Literal LitInt(int x) {
  return (Literal){
      .id = LIT_INTEGER,
      .num = (Num){x},
  };
}

static inline Literal LitUnit(void) {
  return (Literal){
      .id = LIT_UNIT,
  };
}

static inline Literal LitTrue(void) {
  return (Literal){
      .id = LIT_TRUE,
  };
}

static inline Literal LitFalse(void) {
  return (Literal){
      .id = LIT_FALSE,
  };
}

static inline Literal LitString(Array(char) s) {
  return (Literal){
    .id = LIT_STRING,
    .str = s,
  };
}
