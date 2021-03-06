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

  /* leading and/or trailing white-spaces are permitted. */
  assert(pug_parseTest("    1")); /* 1 */
  assert(pug_parseTest("  1  ")); /* 1 */
  assert(pug_parseTest("1    ")); /* 1 */
  assert(pug_parseTest("\n"
                       "let x = 1;\n"
                       "x + 3\n")); /* 4 */

  /* line-comment:
   * `//` followed by any characters and ends with end of line.
   */
  assert(pug_parseTest("1 // (1)")); /* 1 */
  assert(pug_parseTest("// (1)\n"
                       "\n"
                       "// (2)\n"
                       "100\n"
                       "// (3)\n"
                       "\n")); /* 100 */

  /* a variable must be initialized when defining it. */
  assert(pug_parseTest("let a = 100")); /* 100 */

  /* defining the same variable, the previous one will be shadowed. */
  assert(pug_parseTest("let a = 100; let a = 2")); /* 2 */

  /* defining the same variable of different type is NOT permitted. */
  assert(!pug_parseTest("let a = 100; let a = true"));
  /* -> type error: Type mismatch */
  assert(!pug_parseTest("let a = 100; {let a = true}"));
  /* -> type error: Type mismatch */

  /* In different scope,
   * defining the same variable of different type is permitted. */
  assert(pug_parseTest("let a = 100; |x|{let a = true; a}"));
  /* -> ok */
  assert(pug_parseTest("let a = 100; (|x|{let a = true; a}) ()"));
  /* -> true */

  /* TODO what's happened!? */
  assert(!pug_parseTest("let a = 100; |x|{let a = true}"));
  /* -> type error: Type mismatch */
  assert(!pug_parseTest("let a = 100; |x|{x; let a = true}"));
  /* -> type error: Type mismatch */

  /* evaluating a variable results its value. */
  assert(pug_parseTest("let a = 100; a")); /* 100 */

  /* evaluating an undefined variable is not permitted. */
  assert(!pug_parseTest("a")); /* Undefined variable */
  assert(!pug_parseTest("let a = a"));
  /* -> type error: Undefined variable */
  assert(pug_parseTest("var a : a; let a = a")); /* -> (Var a) */
  /* NOTE right-hand side is not evaluated. */

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

  /* a block establishes new **branch** of lexical scope. */
  assert(pug_parseTest("let x = 1; {let x = 2}"));    /* 2 */
  assert(pug_parseTest("let x = 1; {let x = 2}; x")); /* 1 */

  /*
   * From inside a block,
   * variables defined before the block in outer block are accessible.
   */
  assert(pug_parseTest("let x = 1; {let y = x + 1}"));      /* 2 */
  assert(pug_parseTest("let x = 1; {let y = 2; {x + y}}")); /* 3 */

  /*
   * From inside a block,
   * variables defined after the block in outer block are NOT accessible.
   * (block's scope isn't a **nested scope** but a **branched scope**)
   */
  assert(!pug_parseTest("{z}; let z = 2"));
  // -> Undefined variable
  assert(!pug_parseTest("{let y = z}; let z = 2"));
  // -> Undefined variable
  assert(pug_parseTest("var z:a; {let y = z}; let z = 2")); /* 2 */
  // NOTE `z` in the block is never evaluated

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

  /* lexical scoping (static scoping):
   * | a free variable refers the scope where the lambda was defined
   * | instead of where the lambda was applied.
   *
   * NOTE: A term "free variable" means that variables refered in the
   *       lambda's body excluding lambda's argumnets.
   */
  assert(pug_parseTest("let y = 10;\n"
                       "let g = |x| x*y;\n"
                       "let f = |x| {let y = 2; y*y + g x};\n"
                       "f 3 == 34"));
  // -> true
  // NOTE: (f 3) shall be 10 if dynamic scoping.
  // NOTE: (f 3) shall be 34 if lexical scoping.
  // The Pug language employs a lexical scoping.

  /*
   * From inside a lambda (i.e. closure),
   * any variables defined in outer scope are accessible even if the
   * variable was not defined before the lambda.
   * (because closure's scope is a **nested scope**)
   * However it must be declared before at least.
   */
  assert(!pug_parseTest("let f = |x| y; let y = 2; (f 1) == 2"));
  /* -> type error: Undefined variable - y */
  assert(pug_parseTest("var y:a; let f = |x| y; let y = 2; (f 1) == 2"));
  /* -> true */

  /* Recursive function is also available.
   * (because closure's scope is a **nested scope**)
   */
  assert(pug_parseTest("let f = |x| if x <= 1 {1} else {x * f(x-1)};\n"
                       "f 5 == 120"));

  /* logical or (short circuit)
   * | `a || b` results one of the following:
   * | - an error, if either `a` or `b` was not a `bool`.
   * | - `true` if `a` was `true`. (`b` is never evaluated)
   * | - `b` if `a` was `false`
   */
  assert(pug_parseTest("false || false == false"));
  assert(pug_parseTest("false || true  == true"));
  assert(pug_parseTest("true  || false == true"));
  assert(pug_parseTest("true  || true  == true"));
  assert(!pug_parseTest("false || 1")); /* Type mismatch */
  assert(!pug_parseTest("true || 1"));  /* Type mismatch */
  assert(!pug_parseTest("1 || false")); /* Type mismatch */
  assert(!pug_parseTest("1 || true"));  /* Type mismatch */
  assert(!pug_parseTest("1 || 1"));     /* Type mismatch */

  /* logical and (short circuit)
   * | `a && b` results one of the following:
   * | - an error, if either `a` or `b` was not a `bool`.
   * | - `false` if `a` was `false`. (`b` is never evaluated)
   * | - `b` if `a` was `true`.
   */
  assert(pug_parseTest("false && false == false"));
  assert(pug_parseTest("false && true  == false"));
  assert(pug_parseTest("true  && false == false"));
  assert(pug_parseTest("true  && true  == true"));
  assert(!pug_parseTest("false && 1")); /* Type mismatch */
  assert(!pug_parseTest("true  && 1")); /* Type mismatch */
  assert(!pug_parseTest("1 && false")); /* Type mismatch */
  assert(!pug_parseTest("1 && true"));  /* Type mismatch */
  assert(!pug_parseTest("1 && 1"));     /* Type mismatch */

  /* if expr {...} else {...} */
  assert(pug_parseTest("(if true {1} else {0}) == 1"));
  assert(pug_parseTest("(if false {1} else {0}) == 0"));
  assert(pug_parseTest("(if false {2} else if true {1} else {0}) == 1"));
  assert(pug_parseTest("(if false {2} else if false {1} else {0}) == 0"));

  /* nested function definition and recursive call */
  assert(pug_parseTest("// factorial function\n"
                       "let fact = |x| {\n"
                       "  let f = |x a|\n"
                       "    if x <= 1 {\n"
                       "      a\n"
                       "    } else {\n"
                       "      f (x-1) (x*a)\n"
                       "    }\n"
                       "  ;\n"
                       "  f x 1\n"
                       "}\n"
                       ";\n"
                       "fact 4 == 24")); /* true */

  /* print a value of expression. returns `()` */
  /* NOTE: it's tentative and will be removed when I/O library ready. */
  assert(pug_parseTest("let f = |x| 2 * x; print (f 10);"));

  /* operator symbol may be used as a variable */
  assert(pug_parseTest("let (+++) = |a b| a+b; (+++) 2 3 == 5"));

  /* `var VAR : TYPE` declares variable with type annotation */
  assert(pug_parseTest("var x: int"));
  assert(pug_parseTest("var x: bool"));
  assert(pug_parseTest("var x: ()"));
  assert(pug_parseTest("var f : |int int| int;"));
  /* -> a type of a binary function */
  assert(pug_parseTest("var x : Data0;"));
  /* -> a type of a data type */
  assert(pug_parseTest("var x : Data1 a;"));
  /* -> a type of a data type */
  assert(pug_parseTest("var x : Data1 bool;"));
  /* -> a type of a data type */
  assert(pug_parseTest("var x : Data2 f a;"));
  /* -> a type of a data type */
  assert(pug_parseTest("var x : Data2 (|a b| b) a;"));
  /* -> a type of a data type */

  /* if the variable was declared but not defined yet, it cannot be
   * refered. */
  assert(!pug_parseTest("var x: int; x")); /* undefined */

  /* if the variable was declared, its definition must be same type. */
  assert(pug_parseTest("var x: int; let x = 100;")); /* 100 */
  assert(!pug_parseTest("var x: int; let x = true;"));
  /* -> Type mismatch */

  /* if the same variable was declared, the previous one will be shadowed.
   */
  assert(!pug_parseTest("let x = 100;"
                        "var x: bool;"
                        "let x = 10;")); /* Type mismatch */

  assert(!pug_parseTest("let x = 100;"
                        "var x: bool;"
                        "x;")); /* Undefined */

  assert(!pug_parseTest("let x = 100;"
                        "var x: bool;"
                        "x = true;")); /* Undefined */

  assert(pug_parseTest("let x = 100;"
                       "var x: bool;"
                       "let x = true;")); /* true */

  /* type check / type inference for function */
  assert(pug_parseTest("var f : |int| int; let f = |x| x;"));

  /* type check / type inference for generic function */
  assert(pug_parseTest("var f : |a| a; let f = |x| x;"));

  /* match expression */
  assert(pug_parseTest("match 1 { 1 => true; _ => false }"));
  // -> true
  assert(!pug_parseTest("match () { 1 => true; _ => false }"));
  // -> type error
  assert(!pug_parseTest("match 1 { () => true; _ => false }"));
  // -> type error
  assert(!pug_parseTest("match 1 { true => true; _ => false }"));
  // -> type error
  assert(pug_parseTest("type Maybe a = Just a | Nothing;\n"
                       "let unwrap = |m| match m {Just x => x};\n"
                       "unwrap (Just 2) == 2;"
                       ));
  // -> true
  assert(!pug_parseTest("type Maybe a = Just a | Nothing;\n"
                        "let unwrap = |m| match m {Just x => x};\n"
                        "unwrap Nothing; // -> runtime error\n"
                        ));
  // -> runtime error
  assert(pug_parseTest("type Maybe a = Just a | Nothing;\n"
                       "let unwrap = |m| match m {Just x => x};\n"
                       "let f = |m| match m {\n"
                       "  Just x  => Just (2*x);\n"
                       "  Nothing => Nothing\n"
                       "};\n"
                       "f Nothing == Nothing;\n"
                       "unwrap (f (Just 10)) == 20;\n"
                       ));
  // -> true
}
