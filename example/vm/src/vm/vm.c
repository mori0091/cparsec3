/* -*- coding: utf-8-unix -*- */

#include <assert.h>

#include "vm/debug.h"
#include "vm/vm.h"

impl_Mem(Update);
impl_Array(Update);

impl_Mem(Closure);
impl_Array(Closure);

impl_Mem(Cell);
impl_Array(Cell);

/** panic! */
void panic(String msg) {
  printf("panic: %s\n", msg);
  abort();
}

/** garbage collector (not implemented yet) */
static void runGC(void) {
  panic("heap memory exhausted");
}

/** custom made list constructor */
static List(Adr) cons(Adr a, List(Adr) as, CellHeap* cellHeap) {
  if (cellHeap->size >= cellHeap->array.length) {
    runGC();
  }
  List(Adr) bs = &cellHeap->array.data[cellHeap->size++];
  bs->head = a;
  bs->tail = as;
  return bs;
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
static inline AStack push(Adr a, AStack as, CellHeap* cellHeap) {
  return cons(a, as, cellHeap);
}

/** move head of AStack to Env ; <es, a::as> -> <a::es, as> */
static inline void moveHead(Env* dst, AStack* src) {
  assert(dst && src && "null pointer");
  assert(*src && "empty list / stack underflow");
  List(Adr) es = *src;
  *src = es->tail;
  es->tail = *dst;
  *dst = es;
}

/** push/save (for UStack) */
static inline UStack save(AStack as, Adr a, UStack us) {
  if (us.size >= us.array.length) {
    panic("stack overflow");
  }
  Update u = {.as = as, .a = a};
  us.array.data[us.size++] = u;
  return us;
}

/** pop/restore (for UStack) */
static inline UStack restore(AStack* asref, Adr* aref, UStack us) {
  assert(asref && aref && "null pointer");
  if (!us.size) {
    panic("empty list / stack underflow");
  }
  Update u = us.array.data[--us.size];
  *asref = u.as;
  *aref = u.a;
  return us;
}

static inline Adr heapNew(Heap* href, Closure c) {
  if (c.t.tag == VM_VAR) {
    // short circuit (no need to allocate)
    return nth(c.t.n, c.es);
  }
  assert(href && "null pointer");
  if (href->size >= href->array.length) {
    // heap memory is full! now start GC!
    runGC();
  }
  if (c.t.tag == VM_LIT) {
    // literals need not its own environment
    c.es = NULL;
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

static inline VMState runLet(VMState s) {
  Closure v = {.t = *s.c.t.v, .es = s.c.es};
  Closure e = {.t = *s.c.t.e, .es = s.c.es};
  Adr a = heapNew(&s.h, v);
  s.c = e;
  s.c.es = push(a, s.c.es, &s.cellHeap);
  return s;
}

static inline VMState runApp(VMState s) {
  Closure c1 = {.t = *s.c.t.t1, .es = s.c.es};
  Closure c2 = {.t = *s.c.t.t2, .es = s.c.es};
  Adr a = heapNew(&s.h, c2);
  s.c = c1;
  s.as = push(a, s.as, &s.cellHeap);
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
  switch (s.c.t.tag) {
  case VM_LAM:
  case VM_LIT:
    // no need to update
    return s;
  default:
    // schedule an update
    s.us = save(s.as, a, s.us);
    s.as = NULL;
    return s;
  }
}

static inline VMState runUpdate(VMState s) {
  Adr a;
  s.us = restore(&s.as, &a, s.us);
  heapUpdate(&s.h, a, s.c);
  return s;
}

VMState runState(VMState s) {
  switch (s.c.t.tag) {
  case VM_LET:
    return runLet(s);
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
    if (s.us.size) {
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
  s.c.t.tag = VM_LIT;
  s.c.t.i = s.c.t.f(sl.c.t.i, sr.c.t.i);
  s.c.es = NULL; // literals need not its own environment
  return s;
}

VMState evalWHNF(VMState s) {
  for (;;) {
    dumpVMState(s);
    switch (s.c.t.tag) {
    case VM_FN2:
      return runFn2(s);
    case VM_LAM:
      if (!s.as && !s.us.size) {
        return s;
      }
      break;
    case VM_LIT:
      if (!s.us.size) {
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

  CellHeap cellHeap = {
      .array = trait(Array(Cell)).create(256),
      .size = 0,
  };
  UStack us = {
      .array = trait(Array(Update)).create(256),
      .size = 0,
  };
  Heap heap = {
      .array = trait(Array(Closure)).create(256),
      .size = 0,
  };

  VMState s = {
      .c = {t, NULL},
      .as = NULL,
      .us = us,
      .h = heap,
      .cellHeap = cellHeap,
  };
  s = evalWHNF(s);
  dumpVMState(s);
  return s.c.t;
}
