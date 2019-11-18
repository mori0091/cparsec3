/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#include "itr.h"

#define Slice(C) TYPE_NAME(Slice, C)
#define SliceT(C) TYPE_NAME(SliceT, C)

// -----------------------------------------------------------------------
#define trait_Slice(C)                                                   \
  C_API_BEGIN                                                            \
  /* ---- Slice(C) */                                                    \
  typedef struct {                                                       \
    Itr(C) begin;                                                        \
    Itr(C) end;                                                          \
  } Slice(C);                                                            \
  /* ---- trait Slice(C) */                                              \
  typedef struct {                                                       \
    Slice(C) empty;                                                      \
    bool (*null)(Slice(C) s);                                            \
    size_t (*length)(Slice(C) s);                                        \
    Slice(C) (*slice)(C c, size_t start, size_t length);                 \
  } SliceT(C);                                                           \
  SliceT(C) Trait(Slice(C));                                             \
  /* ---- instance Itr(Slice(C)) */                                      \
  typedef Item(C) Item(Slice(C));                                        \
  typedef struct {                                                       \
    Itr(C) itr;                                                          \
    Itr(C) end;                                                          \
  } Itr(Slice(C));                                                       \
  trait_Itr(Slice(C));                                                   \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Slice(C)                                                \
  C_API_BEGIN                                                            \
  /* ---- trait Slice(C) */                                              \
  static bool FUNC_NAME(null, Slice(C))(Slice(C) s) {                    \
    ItrT(C) I = trait(Itr(C));                                           \
    return I.ptr(s.begin) == I.ptr(s.end);                               \
  }                                                                      \
  static size_t FUNC_NAME(length, Slice(C))(Slice(C) s) {                \
    ItrT(C) I = trait(Itr(C));                                           \
    size_t len = 0;                                                      \
    for (; I.ptr(s.begin) != I.ptr(s.end); s.begin = I.next(s.begin)) {  \
      len++;                                                             \
    }                                                                    \
    return len;                                                          \
  }                                                                      \
  static Slice(C)                                                        \
      FUNC_NAME(slice, Slice(C))(C c, size_t start, size_t length) {     \
    ItrT(C) I = trait(Itr(C));                                           \
    Itr(C) begin = I.itr(c);                                             \
    for (; 0 < start && !I.null(begin); begin = I.next(begin)) {         \
      start--;                                                           \
    }                                                                    \
    Itr(C) end = begin;                                                  \
    for (; 0 < length && !I.null(end); end = I.next(end)) {              \
      length--;                                                          \
    }                                                                    \
    return (Slice(C)){.begin = begin, .end = end};                       \
  }                                                                      \
  SliceT(C) Trait(Slice(C)) {                                            \
    Itr(C) it = trait(Itr(C)).itr(trait(C).empty);                       \
    return (SliceT(C)){                                                  \
        .empty = {.begin = it, .end = it},                               \
        .null = FUNC_NAME(null, Slice(C)),                               \
        .length = FUNC_NAME(length, Slice(C)),                           \
        .slice = FUNC_NAME(slice, Slice(C)),                             \
    };                                                                   \
  }                                                                      \
  /* ---- instance Itr(Slice(C))*/                                       \
  static Itr(Slice(C)) FUNC_NAME(itr, Itr(Slice(C)))(Slice(C) s) {       \
    return (Itr(Slice(C))){.itr = s.begin, .end = s.end};                \
  }                                                                      \
  static Item(C) * FUNC_NAME(ptr, Itr(Slice(C)))(Itr(Slice(C)) it) {     \
    return trait(Itr(C)).ptr(it.itr);                                    \
  }                                                                      \
  static Itr(Slice(C))                                                   \
      FUNC_NAME(next, Itr(Slice(C)))(Itr(Slice(C)) it) {                 \
    ItrT(C) I = trait(Itr(C));                                           \
    assert(I.ptr(it.itr) != I.ptr(it.end));                              \
    it.itr = I.next(it.itr);                                             \
    return it;                                                           \
  }                                                                      \
  instance_Itr(Slice(C), FUNC_NAME(itr, Itr(Slice(C))),                  \
               FUNC_NAME(ptr, Itr(Slice(C))),                            \
               FUNC_NAME(next, Itr(Slice(C))));                          \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
