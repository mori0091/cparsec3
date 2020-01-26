/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#define Mem(T) TYPE_NAME(Mem, T)
#define MemT(T) TYPE_NAME(MemT, T)
// -----------------------------------------------------------------------
#define trait_Mem(T)                                                     \
  C_API_BEGIN                                                            \
  typedef struct MemT(T) MemT(T);                                        \
  struct MemT(T) {                                                       \
    T* (*create)(size_t);                                                \
    T* (*recreate)(T*, size_t);                                          \
    void (*free)(T*);                                                    \
  };                                                                     \
  MemT(T) Trait(Mem(T));                                                 \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Mem(T)                                                      \
  C_API_BEGIN                                                            \
  static T* FUNC_NAME(create, Mem(T))(size_t n) {                        \
    return (T*)mem_malloc(n * sizeof(T));                                \
  }                                                                      \
  static T* FUNC_NAME(recreate, Mem(T))(T * ptr, size_t n) {             \
    return (T*)mem_realloc(ptr, n * sizeof(T));                          \
  }                                                                      \
  static void FUNC_NAME(free, Mem(T))(T * p) {                           \
    mem_free((void*)p);                                                  \
  }                                                                      \
  MemT(T) Trait(Mem(T)) {                                                \
    return (MemT(T)){                                                    \
        .create = FUNC_NAME(create, Mem(T)),                             \
        .recreate = FUNC_NAME(recreate, Mem(T)),                         \
        .free = FUNC_NAME(free, Mem(T)),                                 \
    };                                                                   \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
C_API_BEGIN

/**
 * \brief Type of the "memory allocation context" handle.
 */
typedef struct MemCtx* MemCtx;

/**
 * \brief Establishes new memory allocation context.
 *
 * mem_ctx_begin() does following:
 * 1. creates a new memory allocation context,
 * 2. registers it as the child of the current context,
 * 3. establiches it as the current context. and then
 * 4. returns the new established context.
 *
 * \return the handle of the new memory allocation context.
 */
MemCtx mem_ctx_begin(void);
/**
 * \brief Releases and rollbacks the memory allocation context.
 *
 * mem_ctx_end() does following:
 * 1. releases the given memory allocation context `ctx`,
 * 2. releases all memory blocks which were associated to `ctx`,
 * 3. releases all children of the given context recursively,
 * 4. releases all memory blocks which were associated to the children,
 * 5. rollbacks to the given context's parent as the current context.
 *
 * \param ctx   the handle of the memory allocation context.
 */
void mem_ctx_end(MemCtx ctx);

// -----------------------------------------------------------------------
/**
 * \brief Allocates new memory block associated to the current memory
 * allocation context.
 *
 * \param sz   size of the memory block in bytes.
 * \return     the allocated memory block, or NULL if failed.
 *
 * \note if no memory allocation context was established before, a new
 * context was established as the current context at first.
 */
void* mem_malloc(size_t sz);
/**
 * \brief Reallocates the memory block which is previously allocated by
 * mem_malloc() or mem_realloc().
 *
 * \param p    pointer to the memory block which is previously allocated.
 * \param sz   new size of the memory block in bytes.
 * \return     the reallocated memory block, or NULL if not reallocated.
 *
 * \note if no memory allocation context was established before, a new
 * context was established as the current context at first.
 */
void* mem_realloc(void* p, size_t sz);
/**
 * \brief Releases the memory block which is previously allocated by
 * mem_malloc() or mem_realloc().
 *
 * \param p    pointer to the memory block which is previously allocated.
 */
void mem_free(void* p);

// -----------------------------------------------------------------------
#include <stdarg.h>
int mem_asprintf(char** strp, const char* fmt, ...);
int mem_vasprintf(char** strp, const char* fmt, va_list ap);

typedef struct CharBuff CharBuff;
struct CharBuff {
  size_t length;
  char* data;
};

int mem_printf(CharBuff* b, const char* fmt, ...);
int mem_vprintf(CharBuff* b, const char* fmt, va_list ap);

C_API_END
