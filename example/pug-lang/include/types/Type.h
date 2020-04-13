/* -*- coding: utf-8-unix -*- */
#pragma once

#include "user_type.h"

#define TypeId(T) CAT(TypeId_, T)

typedef struct Type {
  enum {
    TypeId(unit),
    TypeId(bool),
    TypeId(int),
  } id;
} Type;

trait_Eq(Type);

typedef struct TypeT {
  Type (*type_unit)(void);
  Type (*type_bool)(void);
  Type (*type_int)(void);
} TypeT;

TypeT Trait(Type);

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

static bool FUNC_NAME(eq, Eq(Type))(Type a, Type b) {
  return a.id == b.id;
}
instance_Eq(Type, FUNC_NAME(eq, Eq(Type)));

static Type FUNC_NAME(type_unit, Type)(void) {
  return (Type){.id = TypeId(unit)};
}
static Type FUNC_NAME(type_bool, Type)(void) {
  return (Type){.id = TypeId(bool)};
}
static Type FUNC_NAME(type_int, Type)(void) {
  return (Type){.id = TypeId(int)};
}

TypeT Trait(Type) {
  return (TypeT){
      .type_unit = FUNC_NAME(type_unit, Type),
      .type_bool = FUNC_NAME(type_bool, Type),
      .type_int = FUNC_NAME(type_int, Type),
  };
}

#endif
