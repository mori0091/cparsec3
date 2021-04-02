/* -*- coding: utf-8-unix -*- */
#pragma once

/* Term Graph Builder API */

#include <vm/stack.h>
#include <vm/term.h>

/**
 * Return the shortest length in `Word`s,
 * that is greater than or equal to `n` in bytes.
 */
#define WORD_LEN(n) (((n) + sizeof(Word) - 1) / sizeof(Word))

/**
 * Return the shortest length in `Word`s,
 * that is greater than or equal to `sizeof(T)` in bytes.
 */
#define WORD_SIZEOF(T) WORD_LEN(sizeof(T))

#define Word int64_t

trait_Stack(Word);

Term* tg_let(Stack(Word) * a, Term* v, Term* e);
Term* tg_app(Stack(Word) * a, Term* m, Term* n);
Term* tg_lam(Stack(Word) * a, Term* e);
Term* tg_var(Stack(Word) * a, size_t n);
Term* tg_lit(Stack(Word) * a, int i);
Term* tg_fn2(Stack(Word) * a, int (*f)(int, int));
