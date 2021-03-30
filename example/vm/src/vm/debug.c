/* -*- coding: utf-8-unix -*- */

#include "vm/debug.h"

inline static void printTerm(Term t) {
  switch (t.tag) {
  case VM_LET:
    printf("LET {v:%p, e:%p}", (void*)t.v, (void*)t.e);
    break;
  case VM_APP:
    printf("APP {t1:%p, t2:%p}", (void*)t.t1, (void*)t.t2);
    break;
  case VM_LAM:
    printf("LAM {t:%p}", (void*)t.t);
    break;
  case VM_VAR:
    printf("VAR {n:%zu}", t.n);
    break;
  case VM_LIT:
    printf("LIT {i:%d}", t.i);
    break;
  case VM_FN2:
    printf("FN2 {f:%p}", (void*)(intptr_t)t.f);
    break;
  default:
    panic("** unknown term **");
    break;
  }
}

static void printEnv(Env es) {
  printf("%p:", (void*)es);
  printf("[");
  if (es) {
    printf("%zu", (size_t)es->head);
    while (es->tail) {
      es = es->tail;
      printf(", %zu", (size_t)es->head);
    }
  }
  printf("]");
}

inline static void printAStack(AStack as) {
  printEnv(as);
}

inline static void printUpdate(Update u) {
  printf("{");
  printAStack(u.as);
  printf(", %zu}", (size_t)u.a);
}

static void printUStack(UStack us) {
  printf("[");
  if (us.size) {
    printUpdate(us.array.data[--us.size]);
    while (us.size) {
      printf(", ");
      printUpdate(us.array.data[--us.size]);
    }
  }
  printf("]");
}

static void dumpHeap(Heap h) {
  printf("vm: heap used %zu / %zu\n", h.size, h.array.length);
  for (size_t a = 0; a < h.size; ++a) {
    printf("%5zu.t : ", a);
    printTerm(h.array.data[a].t);
    printf("\n");
    printf("%5s.es: ", "");
    printEnv(h.array.data[a].es);
    printf("\n");
  }
}

static void dumpCellHeap(CellHeap ch) {
  printf("vm: cell heap used %zu / %zu\n", ch.size, ch.array.length);
  for (size_t i = 0; i < ch.size; ++i) {
    Cell* p = &ch.array.data[i];
    printf("    ");
    printf("%p:{head:%zu, tail:%p}\n", (void*)p, p->head, (void*)p->tail);
  }
}

void dumpVMState(VMState s) {
  printf("vm: c.t : ");
  printTerm(s.c.t);
  printf("\n");
  printf("vm: c.es: ");
  printEnv(s.c.es);
  printf("\n");
  printf("vm:   as: ");
  printAStack(s.as);
  printf("\n");
  printf("vm:   us: ");
  printUStack(s.us);
  printf("\n");
  dumpHeap(s.h);
  dumpCellHeap(s.cellHeap);
  printf("\n");
}
