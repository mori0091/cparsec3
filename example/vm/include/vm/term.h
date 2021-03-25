/* -*- coding: utf-8-unix -*- */
#pragma once

enum TermTag {
  VM_APP, // App M N
  VM_LAM, // Lam E
  VM_VAR, // Var n
  VM_LIT, // Lit i
  VM_FN2, // Fn2 (int x int -> int)
};

typedef struct Term Term;
struct Term {
  enum TermTag tag;
  union {
    struct /* App */ {
      Term* t1;
      Term* t2;
    };
    struct /* Lam */ {
      Term* t;
    };
    struct /* Var */ {
      size_t n;
    };
    struct /* Lit */ {
      int i;
    };
    struct /* Fn2 */ {
      int (*f)(int, int);
    };
  };
};
