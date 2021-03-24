/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#include "prim_int.h"
#include "term.h"

/**
 * Type `Adr` represents a value of index, address, pointer, or
 * reference that refers to a closure stored in the `Heap` memory.
 */
#define Adr size_t

/**
 * Type `Env` represents a scope of accessible variables ;
 * a.k.a. environment of the current state of computation ;
 * that is represented as list of `Adr` ordered from local scope
 * to top-level scope. (i.e. the most recently bounded variable
 * in the local scope is stated the head of current environment)
 */
#define Env List(Adr)

/**
 * Type `AStack` represents arguments of function application ;
 * a.k.a. argument stack ; that is represented as list of `Adr`.
 */
#define AStack List(Adr)

/**
 * Type `UStack` represents a list of scheduled tasks for updating
 * a closure stored in `Heap` memory. The word "update" here means
 * that a closure would be replaced with a value of its reduction
 * form after evaluation.
 */
#define UStack List(Update)

typedef struct Update {
  AStack as; // saved argument stack
  Adr a;     // reference to variable to be updated
} Update;

trait_List(Update);

/**
 * Type `Heap` represents a memory / storage that holds all living
 * closures. Closures are identified by the corresponding value of type
 * `Adr`.
 */
#define Heap Array(Closure)

typedef struct Closure {
  Term t; // an expression / term that is evaluable of the closure.
  Env es; // environment of the closure
} Closure;

trait_Mem(Closure);
trait_Array(Closure);

/**
 * Type `VMState` represents a computation state of virtual machine.
 */
typedef struct VMState {
  Closure c; // a closure that is the last output and/or to be next input
  AStack as; // argument stack
  UStack us; // update stack
  Heap h;    // heap memory
} VMState;

/**
 * Performs single step of evaluation.
 */
VMState runState(VMState s);

/**
 * Performs multi step evaluation recursively until the closure would
 * to be Weak Head Normal Form (WHNF).
 */
VMState evalWHNF(VMState s);

/**
 * Evaluates the given expression with initial state.
 * (i.e. evaluates with empty environment / arguments / heap)
 */
Term testVM(Term t);
