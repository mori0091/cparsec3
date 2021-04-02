/* -*- coding: utf-8-unix -*- */
#pragma once

/* Term Graph Builder Easy macros */

#include "term_graph.h"

/**
 * Setup local buffer that is refered by term_graph builder easy macros.
 * \param n   buffer size in bytes
 */
#define IL_TERM_GRAPH_LOCAL_BUFFER(n)                                    \
  Word _il_code_buf_[WORD_LEN(n)] = {0};                                 \
  Stack(Word) _il_code_ = {                                              \
      .array =                                                           \
          {                                                              \
              .length = WORD_LEN(n),                                     \
              .data = _il_code_buf_,                                     \
          },                                                             \
  }

/* term_graph builder easy macros */
#define let(m, n) tg_let(&_il_code_, (m), (n))
#define app(m, n) tg_app(&_il_code_, (m), (n))
#define lam(m) tg_lam(&_il_code_, (m))
#define var(n) tg_var(&_il_code_, (n))
#define lit(i) tg_lit(&_il_code_, (i))
#define f2(f) tg_fn2(&_il_code_, (f))
