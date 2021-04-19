/* -*- coding: utf-8-unix -*- */

#include <il/term_graph_easy.h>
#include <vm/vm.h>

#include <testit.h>

test("2 + 3 * 7 == 23") {
  IL_TERM_GRAPH_LOCAL_BUFFER(384);
  /* clang-format off */
  Term* t = let(lit(2),
            let(let(lit(3),
                let(lit(7),
                f2(imul))),
            f2(iadd)));
  /* clang-format on */
  Term a = testVM(*t);
  c_assert(a.tag == VM_LIT && a.i == 23);
}

test("(2 + 3) * 7 == 35") {
  IL_TERM_GRAPH_LOCAL_BUFFER(384);
  /* clang-format off */
  Term* t = let(let(lit(2),
                let(lit(3),
                f2(iadd))),
            let(lit(7),
            f2(imul)));
  /* clang-format on */
  Term a = testVM(*t);
  c_assert(a.tag == VM_LIT && a.i == 35);
}
