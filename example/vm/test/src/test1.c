/* -*- coding: utf-8-unix -*- */

#include <testit.h>
#include "vm/vm.h"

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
 */
static Term test1(int (*F)(int, int), int LHS, int RHS) {
  // let f = lam lam F in
  // let x = LHS in
  // let y = RHS in
  // f x y
  Term prg[16] = {0};
  // let f = lam lam F in ...
  prg[0] = (Term){.tag = VM_APP, .t1 = &prg[4], .t2 = &prg[1]};
  // lam lam isub
  prg[1] = (Term){.tag = VM_LAM, .t = &prg[2]};
  prg[2] = (Term){.tag = VM_LAM, .t = &prg[3]};
  prg[3] = (Term){.tag = VM_FN2, .f = F};
  // in ...
  prg[4] = (Term){.tag = VM_LAM, .t = &prg[5]};
  // let x = 10 in ...
  prg[5] = (Term){.tag = VM_APP, .t1 = &prg[7], .t2 = &prg[6]};
  // 10
  prg[6] = (Term){.tag = VM_LIT, .i = LHS};
  // in ...
  prg[7] = (Term){.tag = VM_LAM, .t = &prg[8]};
  // let y = 5 in ...
  prg[8] = (Term){.tag = VM_APP, .t1 = &prg[10], .t2 = &prg[9]};
  // 5
  prg[9] = (Term){.tag = VM_LIT, .i = RHS};
  // in ...
  prg[10] = (Term){.tag = VM_LAM, .t = &prg[11]};
  // f x y
  prg[11] = (Term){.tag = VM_APP, .t1 = &prg[13], .t2 = &prg[12]};
  // y
  prg[12] = (Term){.tag = VM_VAR, .n = 0};
  // f x
  prg[13] = (Term){.tag = VM_APP, .t1 = &prg[15], .t2 = &prg[14]};
  // x
  prg[14] = (Term){.tag = VM_VAR, .n = 1};
  // f
  prg[15] = (Term){.tag = VM_VAR, .n = 2};

  return testVM(prg[0]);
}

test("iadd a b : primitive integer addition\n"
     "let f = \\a b -> iadd a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 13") {
  Term t = test1(iadd, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 13);
}

test("isub a b : primitive integer subtraction\n"
     "let f = \\a b -> isub a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 7") {
  Term t = test1(isub, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 7);
}

test("imul a b : primitive integer multiplication\n"
     "let f = \\a b -> imul a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 30") {
  Term t = test1(imul, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 30);
}

test("idiv a b : primitive integer division\n"
     "let f = \\a b -> idiv a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 3") {
  Term t = test1(idiv, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 3);
}

test("irem a b : primitive integer reminder\n"
     "let f = \\a b -> irem a b in \n"
     "  let x = 10 in \n"
     "    let y = 3 in \n"
     "      f x y \n"
     "  == 1") {
  Term t = test1(irem, 10, 3);
  c_assert(t.tag == VM_LIT && t.i == 1);
}
