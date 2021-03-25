/* -*- coding: utf-8-unix -*- */

#include <assert.h>

#include "vm/vm.h"
#include "vm/debug.h"

impl_List(Update);
impl_Mem(Closure);
impl_Array(Closure);

/** panic! */
void panic(String msg) {
  printf("panic: %s\n", msg);
  abort();
}

/** get n-th element (for Env) */
static Adr nth(size_t n, Env es) {
  while (es) {
    if (!n--) {
      return es->head;
    }
    es = es->tail;
  }
  panic("empty list / stack underflow");
  /* never reach */
  return 0;
}

/** push (for AStack) */
static inline AStack push(Adr a, AStack as) {
  return trait(List(Adr)).cons(a, as);
}

/** move head of AStack to Env ; <es, a::as> -> <a::es, as> */
static inline void moveHead(Env* dst, AStack* src) {
  assert(dst && src && "null pointer");
  assert(*src && "empty list / stack underflow");
  List(Adr) es = *src;
  *src = (*src)->tail;
  es->tail = *dst;
  *dst = es;
}

/** push/save (for UStack) */
static inline UStack save(AStack as, Adr a, UStack us) {
  Update u = {.as = as, .a = a};
  return trait(List(Update)).cons(u, us);
}

/** pop/restore (for UStack) */
static inline UStack restore(AStack* asref, Adr* aref, UStack us) {
  assert(asref && aref && "null pointer");
  if (!us) {
    panic("empty list / stack underflow");
  }
  *asref = us->head.as;
  *aref = us->head.a;
  return trait(List(Update)).drop(1, us);
}

static inline void runGC(void) {
  // GC is not implemented yet.
  panic("heap memory exhausted");
}

static inline Adr heapNew(Heap* href, Closure c) {
  assert(href && "null pointer");
  if (href->size >= href->array.length) {
    // heap memory is full! now start GC!
    runGC();
  }
  size_t a = href->size++;
  href->array.data[a] = c;
  return (Adr)a;
}

static inline Closure heapAt(Heap* href, Adr a) {
  assert(href && "null pointer");
  if (href->size <= a) {
    panic("index out of bounds");
  }
  return href->array.data[(size_t)a];
}

static inline void heapUpdate(Heap* href, Adr a, Closure c) {
  assert(href && "null pointer");
  if (href->size <= a) {
    panic("index out of bounds");
  }
  href->array.data[(size_t)a] = c;
}

static inline VMState runApp(VMState s) {
  Closure c1 = {.t = *s.c.t.t1, .es = s.c.es};
  Closure c2 = {.t = *s.c.t.t2, .es = s.c.es};
  Adr a = heapNew(&s.h, c2);
  s.c = c1;
  s.as = push(a, s.as);
  return s;
}

static inline VMState runLam(VMState s) {
  moveHead(&s.c.es, &s.as);
  s.c.t = *s.c.t.t;
  return s;
}

static inline VMState runAccess(VMState s) {
  Adr a = nth(s.c.t.n, s.c.es);
  s.c = heapAt(&s.h, a);
  s.us = save(s.as, a, s.us);
  s.as = NULL;
  return s;
}

static inline VMState runUpdate(VMState s) {
  Adr a;
  s.us = restore(&s.as, &a, s.us);
  heapUpdate(&s.h, a, s.c);
  return s;
}

VMState runState(VMState s) {
  switch (s.c.t.tag) {
  case VM_APP:
    return runApp(s);
  case VM_VAR:
    return runAccess(s);
  case VM_LAM:
    if (s.as) {
      return runLam(s);
    }
    /* fall through */
  default:
    if (s.us) {
      return runUpdate(s);
    }
  }
  panic("invalid term / state");
  /* never reach */
  return s;
}

inline static VMState runFn2(VMState s) {
  VMState sr = s;
  sr.c.t = (Term){.tag = VM_VAR, .n = 0};
  sr = evalWHNF(sr);
  if (sr.c.t.tag != VM_LIT) {
    panic("invalid term : non-literal rhs");
  }
  VMState sl = s;
  sl.c.t = (Term){.tag = VM_VAR, .n = 1};
  sl = evalWHNF(sl);
  if (sl.c.t.tag != VM_LIT) {
    panic("invalid term : non-literal lhs");
  }
  Term t = {.tag = VM_LIT, .i = s.c.t.f(sl.c.t.i, sr.c.t.i)};
  s.c.t = t;
  return s;
}

VMState evalWHNF(VMState s) {
  for (;;) {
    dumpVMState(s);
    switch (s.c.t.tag) {
    case VM_FN2:
      return runFn2(s);
    case VM_LAM:
      if (!s.as && !s.us) {
        return s;
      }
      break;
    case VM_LIT:
      if (!s.us) {
        return s;
      }
      break;
    default:
      break;
    }
    s = runState(s);
  }
}

#include <cparsec3/base/base_generics.h>

Term testVM(Term t) {
  g_scoped(MemCtx) _ = mem_ctx_begin();
  Heap heap = {.array = trait(Array(Closure)).create(1024), .size = 0};
  VMState s = {.c = {t, NULL}, .as = NULL, .us = NULL, .h = heap};
  s = evalWHNF(s);
  dumpVMState(s);
  return s.c.t;
}