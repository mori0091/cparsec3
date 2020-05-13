/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base_generics.h>

#include "Assump.h"
#include "Scheme.h"
#include "Subst.h"
#include "Type.h"

typedef struct TIState {
  /** current type substitution */
  Subst subst;
  /** next index for creating universal quantified type variable */
  int i;
} TIState;

typedef struct TypeError {
  String msg;
} TypeError;

#define A_STATE_TYPE TIState
#define A_ERROR_TYPE TypeError
#define A_RETURN_TYPES Type, Subst, Scheme

#include "../monad/Action.h"

#define TypeInfer(T) ACTION(T)
#define TIResult(T) A_RESULT(T)
