/* -*- coding: utf-8-unix -*- */
#pragma once

/**
 * \brief Constructs type-name of curried function object.
 *
 * For example:
 *
 * - `Fn(T1, R)`                                 :: 
 *   type-name of a function object that;
 *   - takes one argument of type `T1`,
 *   - returns a value of type `R`.
 *
 * - `Fn(T1, T2, T3, T4, T5, T6, T7, T8, T9, R)` :: 
 *   type-name of a function object that;
 *   - takes 9 arguments of type `T1`, `T2`, ..., and `T9`,
 *   - returns a value of type `R`.
 *
 * Note that a type `Fn(T1, ..., Tn, R)` represents a *curried* function.
 * i.e. :
 * - `Fn(A, B, R)` is equivalent to `Fn(A, Fn(B, R))`,
 * - `Fn(A, B, C, R)` is equivalent to `Fn(A, Fn(B, Fn(C, R)))`,
 * - `Fn(A, B, C, D, R)` is equivalent to `Fn(A, Fn(B, Fn(C, Fn(D, R))))`,
 * - respectively.
 */
#define Fn(T1, ...) internal

/**
 * \brief Apply one or more arguments to a function object `f`.
 *
 * `fn_apply` takes a function object `f` and one or more arguments, and
 * returns the resulting value.

 * If the number of arguments was less than the number of parameters of
 * `f`, `fn_apply` performs *partial function application*.
 *
 * For example, when the `f` was a function of type `Fn(T1, T2, R)`,
 * - `fn_apply(f, t1, t2)` returns a value of type `R`,
 * - `fn_apply(f, t1)` returns a function object of type `Fn(T2, R)`.
 *
 * In other words, `fn_apply(f, a, b, c, d)` is equivalent to
 * `fn_apply(fn_apply(fn_apply(fn_apply(f, a), b), c), d)`
 */
#define fn_apply(f, ...) internal

/**
 * \brief Defines a type of function object (`Fn(T1..., R)`)
 *
 * Note that when you define a type `Fn(T1, T2, T3, T4, R)` by using this
 * macro, all of `Fn(T4, R)`, `Fn(T3, T4, R)`, and `Fn(T2, T3, T4, R)`
 * must be defined before.
 *
 * For example, the below code causes a compile-error:
 * ~~~c
 * typedef_Fn(int, char, String); // define type `Fn(int, char, String)`
 * // -> compile-error
 * ~~~
 *
 * In this case, do as follows instead:
 * ~~~c
 * typedef_Fn(char, String);      // define type `Fn(char, String)`
 * typedef_Fn(int, char, String); // define type `Fn(int, char, String)`
 * ~~~
 *
 * Or use `typedef_Fn_r(...)` macro instead:
 * ~~~c
 * typedef_Fn_r(int, char, String); // define type `Fn(char, String)`
 *                                  // and `Fn(int, char, String)`
 * ~~~
 */
#define typedef_Fn(T1, ...) internal

/**
 * \brief Defines a type of function object (`Fn(T1..., R)`) and all types
 * of its partially applied function object, recursively.
 *
 * For example, the below two code is equivalent.
 *
 * ~~~c
 * typedef_Fn_r(A, B, C, D, E, Ret);
 * ~~~
 *
 * ~~~c
 * typedef_Fn(E, Ret);
 * typedef_Fn(D, E, Ret);
 * typedef_Fn(C, D, E, Ret);
 * typedef_Fn(B, C, D, E, Ret);
 * typedef_Fn(A, B, C, D, E, Ret);
 * ~~~
 */
#define typedef_Fn_r(T1, ...) internal

/**
 * \brief Defines a constructor of a function object of type `Fn(T1..., R)`
 *
 * ~~~c
 * #include <cparsec3/base/base.h>
 *
 * typedef_Fn_r(size_t, char, String);
 *
 * fn(repeat, size_t, char, String) {
 *   uint32_t n = args->e1;  // assign the 1st argument to variable `n`
 *   char c = args->e2;      // assign the 2nd argument to variable `c`
 *   String s = malloc(n+1);
 *   if (n) {
 *     memset(s, c, n);
 *   }
 *   s[n] = '\0';
 *   return s;
 * }
 *
 * int main(void) {
 *   Fn(uint32_t, char, String) f = repeat(); // create function object
 *   Fn(char, String) g = fn_apply(f, 3);     // partial application
 *
 *   String sa = fn_apply(g, 'A');
 *   String sb = fn_apply(g, 'B');
 *   printf("%s\n", sa);                      // -> "AAA"
 *   printf("%s\n", sb);                      // -> "BBB"
 *
 *   free((void*)sa);
 *   free((void*)sb);
 *   free(f.args);              // note that `f.args == g.args`
 *
 *   return 0;
 * }
 * ~~~
 */
#define fn(name, T1, ...) internal


#include "./internal/function.h"
