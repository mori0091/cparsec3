/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

// -----------------------------------------------------------------------
struct MemCtx {
  size_t capacity;
  size_t length;
  void** data;
  MemCtx parent;
};

static MemCtx current_ctx = 0;

static inline MemCtx mem_ctx_new(void) {
  MemCtx ctx = malloc(sizeof(struct MemCtx));
  assert(ctx && "failed to establish new memory allocation context");
  *ctx = (struct MemCtx){0};
  return ctx;
}

static inline void mem_ctx_free(MemCtx ctx) {
  if (ctx->data) {
    size_t n = ctx->length;
    while (n) {
      free(ctx->data[--n]);
    }
    free(ctx->data);
  }
  *ctx = (struct MemCtx){0};
  free(ctx);
}

static inline void mem_ctx_push(void) {
  MemCtx ctx = mem_ctx_new();
  ctx->parent = current_ctx;
  current_ctx = ctx;
}

static inline void mem_ctx_pop(void) {
  if (!current_ctx) {
    return;
  }
  MemCtx ctx = current_ctx;
  current_ctx = ctx->parent;
  mem_ctx_free(ctx);
}

#if defined(__GNUC__)
__attribute__((destructor)) static inline void mem_ctx_free_all(void) {
  while (current_ctx) {
    mem_ctx_pop();
  }
}
#endif

// -----------------------------------------------------------------------
MemCtx mem_ctx_begin(void) {
  mem_ctx_push();
  return current_ctx;
}

void mem_ctx_end(MemCtx ctx) {
  assert(ctx && "Null frame pointer");
  MemCtx parent = ctx->parent;
  while (current_ctx && parent != current_ctx) {
    mem_ctx_pop();
  }
}

// for RAII
void FUNC_NAME(free, MemCtx)(MemCtx* p) {
  assert(p && "Null pointer");
  mem_ctx_end(*p);
  *p = NULL;
}

// -----------------------------------------------------------------------
static inline MemCtx mem_ctx_current(void) {
  if (!current_ctx) {
    mem_ctx_push();
  }
  return current_ctx;
}

#define MEM_CTX_EXPAND_SLOTS (64)
static inline void mem_ctx_ensure(MemCtx ctx) {
  if (!ctx->data) {
    size_t cap = MEM_CTX_EXPAND_SLOTS;
    void* p = malloc(sizeof(void*) * cap);
    assert(p && "No memory");
    ctx->capacity = cap;
    ctx->length = 0;
    ctx->data = p;
    return;
  }
  if (ctx->length == ctx->capacity) {
    size_t cap = ctx->length + MEM_CTX_EXPAND_SLOTS;
    void* p = realloc(ctx->data, sizeof(void*) * cap);
    assert(p && "No memory");
    ctx->capacity = cap;
    ctx->data = p;
  }
}

static inline void* on_malloc(void* p, MemCtx ctx) {
  if (!p) {
    return NULL;
  }
  // add entry
  mem_ctx_ensure(ctx);
  ctx->data[ctx->length++] = p;
  return p;
}

static inline void* on_realloc(void* oldptr, void* newptr, MemCtx ctx) {
  if (!newptr) {
    return NULL;
  }
  // replace entry
  if (!oldptr) {
    return on_malloc(newptr, ctx);
  }
  for (MemCtx c = ctx; c; c = c->parent) {
    size_t n = c->length;
    while (n) {
      n--;
      if (c->data[n] == oldptr) {
        c->data[n] = newptr;
        return newptr;
      }
    }
  }
  // not found
  return on_malloc(newptr, ctx);
}

static inline void on_free(void* p, MemCtx ctx) {
  if (!p) {
    return;
  }
  // remove entry
  for (MemCtx c = ctx; c; c = c->parent) {
    size_t n = c->length;
    while (n) {
      n--;
      if (c->data[n] == p) {
        c->length--;
        c->data[n] = c->data[c->length];
        return;
      }
    }
  }
  // not found
}

// -----------------------------------------------------------------------
void* mem_malloc(size_t sz) {
  return on_malloc(malloc(sz), mem_ctx_current());
}

void* mem_realloc(void* p, size_t sz) {
  return on_realloc(p, realloc(p, sz), mem_ctx_current());
}

void mem_free(void* p) {
  free(p);
  on_free(p, mem_ctx_current());
}

// -----------------------------------------------------------------------
int mem_printf(CharBuff* b, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int ret = mem_vprintf(b, fmt, ap);
  va_end(ap);
  return ret;
}

int mem_vprintf(CharBuff* b, const char* fmt, va_list ap) {
  assert(b && "Null pointer");
  assert(fmt && "Null pointer");

  va_list ap2;

  va_copy(ap2, ap);
  int len = vsnprintf(NULL, 0, fmt, ap2);
  va_end(ap2);
  if (len < 0) {
    return -1;
  }

  char* buf;
  if (!b->data) {
    buf = mem_malloc(len + 1);
    if (!buf) {
      return -1;
    }
    b->data = buf;
    b->length = len;
  } else {
    buf = mem_realloc(b->data, b->length + len + 1);
    if (!buf) {
      return -1;
    }
    b->data = buf;
    buf += b->length;
    b->length += len;
  }

  va_copy(ap2, ap);
  int l = vsnprintf(buf, len + 1, fmt, ap2);
  va_end(ap2);
  if (l != len) {
    return -1;
  }

  return len;
}
