/* -*- coding: utf-8-unix -*- */

#include "puglang.h"

// -----------------------------------------------------------------------
void pug_self_test(void) {
  assert(pug_parseTest("123"));
  assert(pug_parseTest("12*3"));
  assert(pug_parseTest("12/3"));
  assert(pug_parseTest("(12*3)/4"));
  assert(pug_parseTest("(12/3)*4"));
  assert(pug_parseTest("12*(3/4)"));
  assert(pug_parseTest("12/(3*4)"));
  assert(pug_parseTest("(12*3)/(3*4)"));
  assert(pug_parseTest("(12*3)/(34)"));
  assert(!pug_parseTest("(12*3)(3*4)")); /* syntax error */
  assert(!pug_parseTest("(12*3a)"));     /* syntax error */
  assert(pug_parseTest("5+4*3-2/1"));
  assert(pug_parseTest("( 5 + 4 ) * 3 - 2 / 1   "));
  assert(pug_parseTest("-1"));
  assert(pug_parseTest("+1"));
  assert(pug_parseTest("- 1"));
  assert(pug_parseTest("+ 1"));
  assert(pug_parseTest("1 - -1"));
  assert(!pug_parseTest("9999999999999999999")); /* syntax error */
  assert(!pug_parseTest("1 / 0"));               /* division by zero */
  assert(!pug_parseTest("1 % 0"));               /* division by zero */
  assert(pug_parseTest("10 % 3"));
  assert(pug_parseTest("10 == 3"));  /* false */
  assert(pug_parseTest("10 == 10")); /* true */
  assert(pug_parseTest("10 != 3"));  /* true */
  assert(pug_parseTest("10 != 10")); /* false */
  assert(pug_parseTest("1 <= 10"));  /* true */
  assert(pug_parseTest("1 < 10"));   /* true */
  assert(pug_parseTest("1 > 10"));   /* false */
  assert(pug_parseTest("1 >= 10"));  /* false */
  assert(pug_parseTest("!0 == -1"));
  assert(pug_parseTest("!1 == -2"));
  assert(pug_parseTest("!true == false"));
  assert(pug_parseTest("!false == true"));

  /* a variable must be initialized when defining it. */
  assert(pug_parseTest("let a = 100")); /* 100 */

  /* defining the same variable, the previous one will be shadowed. */
  assert(pug_parseTest("let a = 100; let a = 2")); /* 2 */

  /* defining the same variable of different type is permitted. the
   * previous one will be shadowed. */
  assert(pug_parseTest("let a = 100; let a = true")); /* true */

  /* evaluating a variable results its value. */
  assert(pug_parseTest("let a = 100; a")); /* 100 */

  /* evaluating an undefined variable is not permitted. */
  assert(!pug_parseTest("a"));         /* Undefined variable */
  assert(!pug_parseTest("let a = a")); /* Undefined variable */

  /* assignment expression results the value assigned. */
  assert(pug_parseTest("let a = 1; a = 100")); /* 100 */

  /* assignment of different type is not permitted. */
  assert(!pug_parseTest("let a = 1; a = ()")); /* Type mismatch */

  /* comparison operators results `true` or `false` */
  assert(pug_parseTest("1 == 1"));                     /* true */
  assert(pug_parseTest("0 == 1"));                     /* false */
  assert(pug_parseTest("let a = 0 < 1; a == true "));  /* true */
  assert(pug_parseTest("let a = 1 < 1; a == false ")); /* true */

  /* `!expr` results the complement of `expr` if `expr` was integer. */
  assert(pug_parseTest("!0 ")); /* -1 */
  assert(pug_parseTest("!1 ")); /* -2 */

  /* `!expr` results the logical `not` of `expr` if `expr` was bool. */
  assert(pug_parseTest("!true "));  /* false */
  assert(pug_parseTest("!false ")); /* true */

  /* arithmetic operators are left-associative. */
  assert(pug_parseTest("100 + 2 + 3 == (100 + 2) + 3"));
  assert(pug_parseTest("100 - 2 - 3 == (100 - 2) - 3"));
  assert(pug_parseTest("100 * 2 * 3 == (100 * 2) * 3"));
  assert(pug_parseTest("100 / 2 / 5 == (100 / 2) / 5"));

  /* comparison operators are non-associative */
  assert(!pug_parseTest("1 == 2 == 0")); /* syntax error */
  assert(!pug_parseTest("1 != 2 == 1")); /* syntax error */
  assert(!pug_parseTest("2 != 2 == 0")); /* syntax error */
  assert(!pug_parseTest("2 == 2 == 1")); /* syntax error */
  assert(!pug_parseTest("1 <= 2 <= 3")); /* syntax error */
  assert(!pug_parseTest("1 <= 2 < 3"));  /* syntax error */
  assert(!pug_parseTest("1 < 2 <= 3"));  /* syntax error */
  assert(!pug_parseTest("1 < 2 < 3"));   /* syntax error */
  assert(!pug_parseTest("3 >= 2 >= 1")); /* syntax error */
  assert(!pug_parseTest("3 >= 2 > 1"));  /* syntax error */
  assert(!pug_parseTest("3 > 2 >= 1"));  /* syntax error */
  assert(!pug_parseTest("3 > 2 > 1"));   /* syntax error */

  /* assignment operators are non-associative. */
  assert(!pug_parseTest("let a = 0; let b = 0; a = b = 1"));
  /* -> syntax error */

  /* a statement must be declaration, block, or expression. */
  assert(pug_parseTest("let a = 1")); /* 1 */
  assert(pug_parseTest("{1}"));       /* 1 */
  assert(pug_parseTest("1"));         /* 1 */

  /* empty block is not permitted. */
  assert(!pug_parseTest("{}")); /* syntax error */

  /* empty statement is not permitted. */
  assert(!pug_parseTest(";")); /* syntax error */

  /* each statement in a list of statements must be separated by ";". */
  assert(!pug_parseTest("1 2"));   /* syntax error */
  assert(!pug_parseTest("1 {2}")); /* syntax error */
  assert(!pug_parseTest("{1} 2")); /* syntax error */

  /* a list of statements may ends with an optional ";". */
  assert(pug_parseTest("1;"));    /* 1 */
  assert(pug_parseTest("{1;}"));  /* 1 */
  assert(pug_parseTest("{1};"));  /* 1 */
  assert(pug_parseTest("{1;};")); /* 1 */

  /* a list of statements are evaluated in order, and results the last
   * value. */
  assert(pug_parseTest("1;2"));      /* 2 */
  assert(pug_parseTest("1;2;3"));    /* 3 */
  assert(pug_parseTest("{1;2}"));    /* 2 */
  assert(pug_parseTest("{1;2;3}"));  /* 3 */
  assert(pug_parseTest("{1}"));      /* 1 */
  assert(pug_parseTest("{1}; 2"));   /* 2 */
  assert(pug_parseTest("1; {2}"));   /* 2 */
  assert(pug_parseTest("{1}; {2}")); /* 2 */

  assert(pug_parseTest("let a = 1; let b = 2; a + b")); /* 3 */
  assert(pug_parseTest("let a = 1; let b = 2; a"));     /* 1 */
  assert(pug_parseTest("let a = 1; let b = 2; b"));     /* 2 */

  /* a block establishes new 'lexical' scope. */
  assert(pug_parseTest("let x = 1; {x = 2}"));        /* 2 */
  assert(pug_parseTest("let x = 1; {x = 2}; x"));     /* 1 */
  assert(pug_parseTest("let x = 1; {let x = 2}"));    /* 2 */
  assert(pug_parseTest("let x = 1; {let x = 2}; x")); /* 1 */

  /*
   * From inside a block,
   * variables defined before the block in outer scope are accessible.
   */
  assert(pug_parseTest("let x = 1; {let y = x + 1}"));      /* 2 */
  assert(pug_parseTest("let x = 1; {let y = 2; {x + y}}")); /* 3 */

  /*
   * From inside a block,
   * variables defined after the block in outer scope are NOT accessible.
   * NOTE: Shall be accessible? Need concideration which is better.
   */
  assert(!pug_parseTest("{let y = z}; let z = 2"));
  // -> Undefined variable

  /*
   * From outside a block,
   * variables defined in the block are NOT accessible.
   */
  assert(!pug_parseTest("{let y = 2}; y"));
  // -> Undefined variable

  /* block is an expression */
  assert(pug_parseTest("let x = {1}; x"));                /* 1 */
  assert(pug_parseTest("let x = {1} + {2}; x"));          /* 3 */
  assert(pug_parseTest("let x = {let y = 1}; x"));        /* 1 */
  assert(pug_parseTest("let x = {let y = 1; y + 1}; x")); /* 2 */

  /* lambda expression constructs a function object. */
  assert(pug_parseTest("|x| x+1")); /* (Lambda (Var x) (Add (Var x) 1)) */
  assert(pug_parseTest("let f = |x| x+1; f 2"));       /* 3 */
  assert(pug_parseTest("let f = |x y| x+y; f 1 2"));   /* 3 */
  assert(pug_parseTest("let f = |x y| {x+y}; f 1 2")); /* 3 */

  /* Function application expression has higher priority than lambda
   * expression.
   */
  assert(pug_parseTest("|x y| x+y 1 2"));     /* |x y| (x + (y 1 2)) */
  assert(!pug_parseTest("|x y| {x+y} 1 2"));  /* syntax error */
  assert(pug_parseTest("(|x y| x+y) 1 2"));   /* 3 */
  assert(pug_parseTest("(|x y| {x+y}) 1 2")); /* 3 */

  /* function object is a curried function. */
  assert(pug_parseTest("let f = |x y| x+y; let g = f 1; g 2"));   /* 3 */
  assert(pug_parseTest("let f = |x| |y| x+y; let g = f 1; g 2")); /* 3 */
  assert(pug_parseTest("let f = |x y| x+y; let g = f 1; g 2 + g 3"));
  // -> (1+2) + (1+3) = 7

  assert(pug_parseTest("let y = 10;\n"
                       "let g = |x| x*y;\n"
                       "let f = |x| {y = 2; y*y + g x};\n"
                       "f 3"));
  // -> 10
  // NOTE: (f 3) shall be 10 if dynamic scoping.
  // NOTE: (f 3) shall be 34 if lexical scoping.
  // TODO: scope shall be established in compile-time not in run-time,
  //       if the lexical scoping was expected.
}
