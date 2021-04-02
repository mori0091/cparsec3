/* -*- coding: utf-8-unix -*- */

#include <il/term_graph.h>

static Term* term(Stack(Word) * a, Term t) {
  assert(a && "null pointer");
  assert(a->size + WORD_SIZEOF(Term) <= a->array.length);
  Term* p = (Term*)&(a->array.data[a->size]);
  *p = t;
  a->size += WORD_SIZEOF(Term);
  return p;
}

Term* tg_let(Stack(Word) * a, Term* v, Term* e) {
  return term(a, (Term){.tag = VM_LET, .v = v, .e = e});
}

Term* tg_app(Stack(Word) * a, Term* m, Term* n) {
  return term(a, (Term){.tag = VM_APP, .t1 = m, .t2 = n});
}

Term* tg_lam(Stack(Word) * a, Term* e) {
  return term(a, (Term){.tag = VM_LAM, .t = e});
}

Term* tg_var(Stack(Word) * a, size_t n) {
  return term(a, (Term){.tag = VM_VAR, .n = n});
}

Term* tg_lit(Stack(Word) * a, int i) {
  return term(a, (Term){.tag = VM_LIT, .i = i});
}

Term* tg_fn2(Stack(Word) * a, int (*f)(int, int)) {
  return term(a, (Term){.tag = VM_FN2, .f = f});
}
