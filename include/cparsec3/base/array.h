/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#include "eq.h"
#include "ord.h"

#include "itr.h"

#define Array(T) TYPE_NAME(Array, T)
#define ArrayT(T) TYPE_NAME(ArrayT, T)

// -----------------------------------------------------------------------
#define trait_Array(T)                                                   \
  C_API_BEGIN                                                            \
  /* ---- Array(T) */                                                    \
  typedef struct {                                                       \
    size_t length;                                                       \
    T* data;                                                             \
  } Array(T);                                                            \
  /* ---- trait Array(T) */                                              \
  typedef struct {                                                       \
    Array(T) empty;                                                      \
    bool (*null)(Array(T) a);                                            \
    size_t (*length)(Array(T) a);                                        \
    Array(T) (*from_array)(size_t n, T* a);                              \
    Array(T) (*create)(size_t n);                                        \
    void (*free)(Array(T) a);                                            \
    T* (*begin)(Array(T) a);                                             \
    T* (*end)(Array(T) a);                                               \
  } ArrayT(T);                                                           \
  ArrayT(T) Trait(Array(T));                                             \
  /* ---- instance Eq(Array(T)) */                                       \
  trait_Eq(Array(T));                                                    \
  /* ---- instance Ord(Array(T)) */                                      \
  trait_Ord(Array(T));                                                   \
  /* ---- instance Itr(Array(T)) */                                      \
  typedef T Item(Array(T));                                              \
  typedef struct {                                                       \
    Array(T) a;                                                          \
  } Itr(Array(T));                                                       \
  trait_Itr(Array(T));                                                   \
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
  static Array(T) FUNC_NAME(create, Array(T))(size_t n) {                \
    return (Array(T)){.length = n, .data = trait(Mem(T)).create(n)};     \
  }                                                                      \
  static Array(T) FUNC_NAME(from_array, Array(T))(size_t n, T * a) {     \
    Array(T) x = FUNC_NAME(create, Array(T))(n);                         \
    for (size_t i = 0; i < n; i++) {                                     \
      x.data[i] = a[i];                                                  \
    }                                                                    \
    return x;                                                            \
  }                                                                      \
  static void FUNC_NAME(free, Array(T))(Array(T) a) {                    \
    trait(Mem(T)).free(a.data);                                          \
  }                                                                      \
  static T* FUNC_NAME(begin, Array(T))(Array(T) a) {                     \
    return a.data;                                                       \
  }                                                                      \
  static T* FUNC_NAME(end, Array(T))(Array(T) a) {                       \
    return a.data + a.length;                                            \
  }                                                                      \
  ArrayT(T) Trait(Array(T)) {                                            \
    return (ArrayT(T)){                                                  \
        .empty = {0},                                                    \
        .null = FUNC_NAME(null, Array(T)),                               \
        .length = FUNC_NAME(length, Array(T)),                           \
        .create = FUNC_NAME(create, Array(T)),                           \
        .from_array = FUNC_NAME(from_array, Array(T)),                   \
        .free = FUNC_NAME(free, Array(T)),                               \
        .begin = FUNC_NAME(begin, Array(T)),                             \
        .end = FUNC_NAME(end, Array(T)),                                 \
    };                                                                   \
  }                                                                      \
  /* ---- instance Eq(Array(T)) */                                       \
  static bool FUNC_NAME(eq, Eq(Array(T)))(Array(T) a, Array(T) b) {      \
    if (a.length != b.length) {                                          \
      return false;                                                      \
    }                                                                    \
    if (a.data == b.data) {                                              \
      return true;                                                       \
    }                                                                    \
    for (size_t i = 0; i < a.length; ++i) {                              \
      if (trait(Eq(T)).neq(a.data[i], b.data[i])) {                      \
        return false;                                                    \
      }                                                                  \
    }                                                                    \
    return true;                                                         \
  }                                                                      \
  instance_Eq(Array(T), FUNC_NAME(eq, Eq(Array(T))));                    \
  /* ---- instance Ord(Array(T)) */                                      \
  static int FUNC_NAME(cmp, Ord(Array(T)))(Array(T) a, Array(T) b) {     \
    if (a.data == b.data) {                                              \
      return trait(Ord(uint64_t)).cmp(a.length, b.length);               \
    }                                                                    \
    size_t n = (a.length <= b.length ? a.length : b.length);             \
    for (size_t i = 0; i < n; ++i) {                                     \
      int o = trait(Ord(T)).cmp(a.data[i], b.data[i]);                   \
      if (o) {                                                           \
        return o;                                                        \
      }                                                                  \
    }                                                                    \
    return (a.length < b.length ? -1 : 1);                               \
  }                                                                      \
  instance_Ord(Array(T), FUNC_NAME(cmp, Ord(Array(T))));                 \
  /* ---- instance Itr(Array(T))*/                                       \
  static Itr(Array(T)) FUNC_NAME(itr, Itr(Array(T)))(Array(T) a) {       \
    return (Itr(Array(T))){.a = a};                                      \
  }                                                                      \
  static T* FUNC_NAME(ptr, Itr(Array(T)))(Itr(Array(T)) it) {            \
    return (it.a.length ? it.a.data : 0);                                \
  }                                                                      \
  static Itr(Array(T))                                                   \
      FUNC_NAME(next, Itr(Array(T)))(Itr(Array(T)) it) {                 \
    if (it.a.length) {                                                   \
      assert(it.a.data);                                                 \
      it.a.length--;                                                     \
      it.a.data++;                                                       \
    }                                                                    \
    return it;                                                           \
  }                                                                      \
  instance_Itr(Array(T), FUNC_NAME(itr, Itr(Array(T))),                  \
               FUNC_NAME(ptr, Itr(Array(T))),                            \
               FUNC_NAME(next, Itr(Array(T))));                          \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
