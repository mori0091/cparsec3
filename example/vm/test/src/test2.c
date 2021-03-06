/* -*- coding: utf-8-unix -*- */

#include <il/term_graph_easy.h>
#include <vm/vm.h>

#include <testit.h>

/**
 * \brief Tests the code returns expected value.
 *
 * This testcase run the VM code similar to the below Haskell code:
 * ~~~haskell
 * let f = \\a b -> F a b in
 *   let x = LHS in
 *     let y = RHS in
 *       f x y
 * --- it should satisfy (EXPECT == F LHS RHS)
 * ~~~
 *
 * \param F        a binary function for `int` type
 * \param LHS      left hand side argument
 * \param RHS      right hand side argument
 * \return         evaluated result
 *
 * \note This test2() is same as test1() but using `LET v e`
 *       instead of `APP (LAM e) v`.
 */
static Term test2(int (*F)(int, int), int LHS, int RHS) {
  IL_TERM_GRAPH_LOCAL_BUFFER(384);
  /* clang-format off */
  Term* prg = let(lam(lam(f2(F))),             // let f = lam lam F in
              let(lit(LHS),                    // let x = LHS in
              let(lit(RHS),                    // let y = RHS in
              app(app(var(2), var(1)), var(0)) // f x y
              )));
  /* clang-format on */
  return testVM(*prg);
}

test("iadd a b : primitive integer addition\n"
     "let f = \\a b -> iadd a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 13") {
  Term t = test2(iadd, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 13);
}

test("isub a b : primitive integer subtraction\n"
     "let f = \\a b -> isub a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 7") {
  Term t = test2(isub, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 7);
}

test("imul a b : primitive integer multiplication\n"
     "let f = \\a b -> imul a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 30") {
  Term t = test2(imul, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 30);
}

test("idiv a b : primitive integer division\n"
     "let f = \\a b -> idiv a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 3") {
  Term t = test2(idiv, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 3);
}

test("irem a b : primitive integer reminder\n"
     "let f = \\a b -> irem a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 1") {
  Term t = test2(irem, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 1);
}
