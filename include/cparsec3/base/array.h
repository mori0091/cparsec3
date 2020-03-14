/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "eq.h"
#include "ord.h"

#include "iterable.h"
#include "itr.h"
#include "slice.h"

#define Array(T) TYPE_NAME(Array, T)
#define ArrayT(T) TYPE_NAME(ArrayT, T)

// -----------------------------------------------------------------------
#define trait_Array(T)                                                   \
  C_API_BEGIN                                                            \
  /* ---- Array(T) */                                                    \
  typedef struct Array(T) Array(T);                                      \
  struct Array(T) {                                                      \
    size_t length;                                                       \
    T* data;                                                             \
  };                                                                     \
  /* ---- trait Array(T) */                                              \
  typedef struct ArrayT(T) ArrayT(T);                                    \
  struct ArrayT(T) {                                                     \
    Array(T) empty;                                                      \
    bool (*null)(Array(T) a);                                            \
    size_t (*length)(Array(T) a);                                        \
    void (*reverse)(Array(T) * pa);                                      \
    void (*free)(Array(T) * pa);                                         \
    Array(T) (*create)(size_t n);                                        \
    T* (*begin)(Array(T) a);                                             \
    T* (*end)(Array(T) a);                                               \
    Array(T) (*from_array)(size_t n, T * a);                             \
  };                                                                     \
  ArrayT(T) Trait(Array(T));                                             \
  /* ---- instance Itr(Array(T)) */                                      \
  typedef T Item(Array(T));                                              \
  typedef struct Itr(Array(T)) Itr(Array(T));                            \
  struct Itr(Array(T)) {                                                 \
    Array(T) a;                                                          \
  };                                                                     \
  trait_Itr(Array(T));                                                   \
  /* ---- instance Iterable(Array(T)) */                                 \
  trait_Iterable(Array(T));                                              \
  /* ---- instance Slice(Array(T)) */                                    \
  typedef struct Slice(Array(T)) Slice(Array(T));                        \
  struct Slice(Array(T)) {                                               \
    Array(T) a;                                                          \
  };                                                                     \
  trait_Slice(Array(T));                                                 \
  /* ---- instance Itr(Slice(Array(T))) */                               \
  typedef T Item(Slice(Array(T)));                                       \
  typedef struct Itr(Slice(Array(T))) Itr(Slice(Array(T)));              \
  struct Itr(Slice(Array(T))) {                                          \
    Itr(Array(T)) it;                                                    \
  };                                                                     \
  trait_Itr(Slice(Array(T)));                                            \
  /* ---- instance Iterable(Slice(Array(T))) */                          \
  trait_Iterable(Slice(Array(T)));                                       \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Array(T)                                                    \
  C_API_BEGIN                                                            \
  /* ---- trait Array(T) */                                              \
  static bool FUNC_NAME(null, Array(T))(Array(T) a) {                    \
    return !a.length;                                                    \
  }                                                                      \
  static size_t FUNC_NAME(length, Array(T))(Array(T) a) {                \
    return a.length;                                                     \
  }                                                                      \
  static void FUNC_NAME(reverse, Array(T))(Array(T) * pa) {              \
    assert(pa);                                                          \
    if (pa->length <= 1) {                                               \
      return;                                                            \
    }                                                                    \
    T* p = pa->data;                                                     \
    T* q = p + pa->length - 1;                                           \
    while (p < q) {                                                      \
      T x = *p;                                                          \
      *p++ = *q;                                                         \
      *q-- = x;                                                          \
    }                                                                    \
  }                                                                      \
  void FUNC_NAME(free, Array(T))(Array(T) * pa) {                        \
    if (pa && pa->data) {                                                \
      trait(Mem(T)).free(pa->data);                                      \
      *pa = (Array(T)){0};                                               \
    }                                                                    \
  }                                                                      \
  static Array(T) FUNC_NAME(create, Array(T))(size_t n) {                \
    return (Array(T)){.length = n, .data = trait(Mem(T)).create(n)};     \
  }                                                                      \
  static T* FUNC_NAME(begin, Array(T))(Array(T) a) {                     \
    return a.data;                                                       \
  }                                                                      \
  static T* FUNC_NAME(end, Array(T))(Array(T) a) {                       \
    return a.data + a.length;                                            \
  }                                                                      \
  static Array(T) FUNC_NAME(from_array, Array(T))(size_t n, T * a) {     \
    Array(T) x = FUNC_NAME(create, Array(T))(n);                         \
    for (size_t i = 0; i < n; i++) {                                     \
      x.data[i] = a[i];                                                  \
    }                                                                    \
    return x;                                                            \
  }                                                                      \
  ArrayT(T) Trait(Array(T)) {                                            \
    return (ArrayT(T)){                                                  \
        .empty = {0},                                                    \
        .null = FUNC_NAME(null, Array(T)),                               \
        .length = FUNC_NAME(length, Array(T)),                           \
        .reverse = FUNC_NAME(reverse, Array(T)),                         \
        .free = FUNC_NAME(free, Array(T)),                               \
        .create = FUNC_NAME(create, Array(T)),                           \
        .begin = FUNC_NAME(begin, Array(T)),                             \
        .end = FUNC_NAME(end, Array(T)),                                 \
        .from_array = FUNC_NAME(from_array, Array(T)),                   \
    };                                                                   \
  }                                                                      \
  /* ---- instance Itr(Array(T))*/                                       \
  static T* FUNC_NAME(ptr, Itr(Array(T)))(Itr(Array(T)) it) {            \
    return it.a.data;                                                    \
  }                                                                      \
  static Itr(Array(T))                                                   \
      FUNC_NAME(next, Itr(Array(T)))(Itr(Array(T)) it) {                 \
    assert(it.a.length&& it.a.data);                                     \
    if (--it.a.length) {                                                 \
      it.a.data++;                                                       \
    } else {                                                             \
      it.a.data = 0;                                                     \
    }                                                                    \
    return it;                                                           \
  }                                                                      \
  instance_Itr(Array(T), FUNC_NAME(ptr, Itr(Array(T))),                  \
               FUNC_NAME(next, Itr(Array(T))));                          \
  /* ---- instance Iterable(Array(T))*/                                  \
  static Itr(Array(T)) FUNC_NAME(itr, Iterable(Array(T)))(Array(T) a) {  \
    return (Itr(Array(T))){.a = a};                                      \
  }                                                                      \
  instance_Iterable(Array(T), FUNC_NAME(itr, Iterable(Array(T))));       \
  /* ---- instance Slice(Array(T)) */                                    \
  static bool FUNC_NAME(null, Slice(Array(T)))(Slice(Array(T)) s) {      \
    return FUNC_NAME(null, Array(T))(s.a);                               \
  }                                                                      \
  static size_t FUNC_NAME(length, Slice(Array(T)))(Slice(Array(T)) s) {  \
    return FUNC_NAME(length, Array(T))(s.a);                             \
  }                                                                      \
  static void FUNC_NAME(reverse,                                         \
                        Slice(Array(T)))(Slice(Array(T)) * ps) {         \
    assert(ps);                                                          \
    FUNC_NAME(reverse, Array(T))(&(ps->a));                              \
  }                                                                      \
  static Slice(Array(T)) FUNC_NAME(slice, Slice(Array(T)))(              \
      Array(T) a, int start, int stop) {                                 \
    size_t len = a.length;                                               \
    size_t s1 = adjust_index(start, len);                                \
    size_t s2 = adjust_index(stop, len);                                 \
    if (s1 >= s2) {                                                      \
      return (Slice(Array(T))){.a = {0}};                                \
    }                                                                    \
    return (Slice(Array(T))){                                            \
        .a.length = s2 - s1,                                             \
        .a.data = a.data + s1,                                           \
    };                                                                   \
  }                                                                      \
  SliceT(Array(T)) Trait(Slice(Array(T))) {                              \
    return (SliceT(Array(T))){                                           \
        .empty = {.a = {0}},                                             \
        .null = FUNC_NAME(null, Slice(Array(T))),                        \
        .length = FUNC_NAME(length, Slice(Array(T))),                    \
        .reverse = FUNC_NAME(reverse, Slice(Array(T))),                  \
        .slice = FUNC_NAME(slice, Slice(Array(T))),                      \
    };                                                                   \
  }                                                                      \
  /* ---- instance Itr(Slice(Array(T))) */                               \
  static T* FUNC_NAME(ptr,                                               \
                      Itr(Slice(Array(T))))(Itr(Slice(Array(T))) it) {   \
    return FUNC_NAME(ptr, Itr(Array(T)))(it.it);                         \
  }                                                                      \
  static Itr(Slice(Array(T)))                                            \
      FUNC_NAME(next, Itr(Slice(Array(T))))(Itr(Slice(Array(T))) it) {   \
    it.it = FUNC_NAME(next, Itr(Array(T)))(it.it);                       \
    return it;                                                           \
  }                                                                      \
  instance_Itr(Slice(Array(T)), FUNC_NAME(ptr, Itr(Slice(Array(T)))),    \
               FUNC_NAME(next, Itr(Slice(Array(T)))));                   \
  /* ---- instance Iterable(Slice(Array(T))) */                          \
  static Itr(Slice(Array(T)))                                            \
      FUNC_NAME(itr, Iterable(Slice(Array(T))))(Slice(Array(T)) s) {     \
    return (Itr(Slice(Array(T)))){                                       \
        .it = FUNC_NAME(itr, Iterable(Array(T)))(s.a),                   \
    };                                                                   \
  }                                                                      \
  instance_Iterable(Slice(Array(T)),                                     \
                    FUNC_NAME(itr, Iterable(Slice(Array(T)))));          \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
