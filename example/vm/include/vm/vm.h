/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#include "prim_int.h"
#include "stack.h"
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
#define UStack Stack(Update)

typedef struct Update {
  AStack as; // saved argument stack
  Adr a;     // reference to variable to be updated
} Update;

trait_Mem(Update);
trait_Array(Update);
trait_Stack(Update); // see "stack.h"

/**
 * Type `Closure` represents a closure that is a pair of expression /
 * term and environment (scope of accessible variables).
 */
typedef struct Closure {
  Term t; // an expression / term that is evaluable of the closure.
  Env es; // environment of the closure
} Closure;

trait_Mem(Closure);
trait_Array(Closure);
trait_Stack(Closure); // see "stack.h"

/**
 * Type `Heap` represents a memory / storage that holds all accessible
 * closures. Closures are identified by the corresponding value of type
 * `Adr`.
 */
#define Heap Stack(Closure)

/**
 * Type `Cell` represents an element container of `List(Adr)`.
 *
 * \note `Cell` and `CellHeap` are used to define a custom made list
 *       constructor.
 */
#define Cell stList(Adr)

trait_Mem(Cell);
trait_Array(Cell);
trait_Stack(Cell); // see "stack.h"

/**
 * Type `CellHeap` represents a memory / storage that holds all accessible
 * memory cells of lists.
 *
 * In other words, `CellHeap` is a set of all memory cells refered by all
 * accessible environments and argument stack.
 */
#define CellHeap Stack(Cell)

/**
 * Type `VMState` represents a computation state of virtual machine.
 */
typedef struct VMState {
  Closure c; // a closure that is the last output and/or to be next input
  AStack as; // argument stack
  UStack us; // update stack
  Heap h;    // heap memory (set of accessible closures)
  CellHeap cellHeap; // set of accessible memory cells for lists
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

void panic(String msg);
