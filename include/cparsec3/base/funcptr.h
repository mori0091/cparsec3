/* -*- coding: utf-8-unix -*- */
#pragma once

#include "function.h"

/**
 * \brief Constructs type-name of function pointer.
 *
 * For example:
 *
 * - `Fn(T1, R)`                                 ::
 *   type-name of a function pointer that;
 *   - takes one argument of type `T1`,
 *   - returns a value of type `R`.
 *
 * - `Fn(T1, T2, T3, T4, T5, T6, T7, T8, T9, R)` ::
 *   type-name of a function pointer that;
 *   - takes 9 arguments of type `T1`, `T2`, ..., and `T9`,
 *   - returns a value of type `R`.
 */
#define Fp(T1, ...) internal

/**
 * \brief Defines a type of function pointer (`Fn(T1..., R)`)
 *
 * For example:
 * ~~~c
 * typedef_Fp(int, char, String);
 * // -> typedef String (*Fp(int, char, String))(int, char);
 *
 * bool is_digit(char c);
 * Fp(char, bool) f = is_digit;
 * ~~~
 */
#define typedef_Fp(T1, ...) internal

/**
 * \brief Defines a function that converts a function pointer to a
 * function object.
 *
 * For example:
 * ~~~c
 * #include <assert.h>
 *
 * typedef_Fn_r(char, bool);
 * fp(make_Fn_from_Fp, char, bool);
 *
 * bool is_digit(char c) {
 *   return ((char)'0' <= c && c <= (char)'9');
 * }
 *
 * int main(void) {
 *   Fn(char, bool) f = make_Fn_from_Fp(is_digit);
 *   assert(fn_apply(f, (char)'1'));
 *   return 0;
 * }
 * ~~~
 */
#define fp(name, T1, ...) internal

#include "internal/funcptr.h"
