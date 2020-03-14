/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

// -----------------------------------------------------------------------
#define ErrorItem(T) TYPE_NAME(ErrorItem, T)

// -----------------------------------------------------------------------
#define trait_ErrorItem(T)                                               \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ErrorItem(T) ErrorItem(T);                              \
  struct ErrorItem(T) {                                                  \
    enum {                                                               \
      LABEL,                                                             \
      TOKENS,                                                            \
      END_OF_INPUT,                                                      \
    } type;                                                              \
    union {                                                              \
      String label;                                                      \
      List(T) tokens;                                                    \
    };                                                                   \
  };                                                                     \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define impl_ErrorItem(T)                                                \
  C_API_BEGIN                                                            \
                                                                         \
  static inline bool FUNC_NAME(isUnknown,                                \
                               ErrorItem(T))(ErrorItem(T) e) {           \
    switch (e.type) {                                                    \
    case LABEL:                                                          \
      return !e.label;                                                   \
    case TOKENS:                                                         \
      return !e.tokens;                                                  \
    case END_OF_INPUT:                                                   \
      return false;                                                      \
    default:                                                             \
      return true;                                                       \
    }                                                                    \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define ParseError(S) TYPE_NAME(ParseError, S)

// -----------------------------------------------------------------------
#define trait_ParseError(S)                                              \
  C_API_BEGIN                                                            \
                                                                         \
  trait_ErrorItem(Token(S));                                             \
  trait_Maybe(ErrorItem(Token(S)));                                      \
  typedef ErrorItem(Token(S)) Item(List(ErrorItem(Token(S))));           \
  trait_List(ErrorItem(Token(S)));                                       \
                                                                         \
  typedef struct ParseError(S) ParseError(S);                            \
  struct ParseError(S) {                                                 \
    Offset offset;                                                       \
    Maybe(ErrorItem(Token(S))) unexpected;                               \
    List(ErrorItem(Token(S))) expecting;                                 \
  };                                                                     \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParseError(S)                                               \
  C_API_BEGIN                                                            \
                                                                         \
  impl_ErrorItem(Token(S));                                              \
  /* impl_Maybe(ErrorItem(Token(S))); */                                 \
  impl_List(ErrorItem(Token(S)));                                        \
                                                                         \
  static inline bool FUNC_NAME(isUnknown,                                \
                               ParseError(S))(ParseError(S) e) {         \
    return ((e.unexpected.none ||                                        \
             FUNC_NAME(isUnknown,                                        \
                       ErrorItem(Token(S)))(e.unexpected.value)) &&      \
            !e.expecting);                                               \
  }                                                                      \
                                                                         \
  static inline void FUNC_NAME(print, ErrorItem(Token(S)))(              \
      ErrorItem(Token(S)) e) {                                           \
    switch (e.type) {                                                    \
    case LABEL:                                                          \
      printf("%s", e.label);                                             \
      break;                                                             \
    case TOKENS:;                                                        \
      String s = trait(Stream(S)).showTokens(e.tokens);                  \
      printf("%s", s);                                                   \
      mem_free((void*)s);                                                \
      break;                                                             \
    case END_OF_INPUT:                                                   \
      printf("end of input");                                            \
      break;                                                             \
    default:                                                             \
      break;                                                             \
    }                                                                    \
  }                                                                      \
  static inline void FUNC_NAME(print, ParseError(S))(ParseError(S) e) {  \
    /* printf("error:%" PRIdMAX ":\n", e.offset); */                     \
    if (FUNC_NAME(isUnknown, ParseError(S))(e)) {                        \
      printf("unknown error\n");                                         \
      return;                                                            \
    }                                                                    \
    void (*printErrorItem)(ErrorItem(Token(S)) t) =                      \
        FUNC_NAME(print, ErrorItem(Token(S)));                           \
    if (!e.unexpected.none) {                                            \
      printf("unexpected ");                                             \
      printErrorItem(e.unexpected.value);                                \
      printf("\n");                                                      \
    }                                                                    \
    if (e.expecting) {                                                   \
      printf("expecting ");                                              \
      ListT(ErrorItem(Token(S))) L = trait(List(ErrorItem(Token(S))));   \
      List(ErrorItem(Token(S))) xs = e.expecting;                        \
      if (!xs) {                                                         \
        printf("\n");                                                    \
        return;                                                          \
      }                                                                  \
      printErrorItem(L.head(xs));                                        \
      xs = L.tail(xs);                                                   \
      if (!xs) {                                                         \
        printf("\n");                                                    \
        return;                                                          \
      }                                                                  \
      if (!L.tail(xs)) {                                                 \
        printf(" or ");                                                  \
        printErrorItem(L.head(xs));                                      \
        printf("\n");                                                    \
        return;                                                          \
      }                                                                  \
      for (; L.tail(xs); xs = L.tail(xs)) {                              \
        printf(", ");                                                    \
        printErrorItem(L.head(xs));                                      \
      }                                                                  \
      printf(", or ");                                                   \
      printErrorItem(L.head(xs));                                        \
      printf("\n");                                                      \
    }                                                                    \
  }                                                                      \
                                                                         \
  static inline Hints(Token(S))                                          \
      FUNC_NAME(toHints, Token(S))(Token(S) t) {                         \
    ErrorItem(Token(S)) item = {                                         \
        .type = TOKENS,                                                  \
        .tokens = trait(List(Token(S))).cons(t, NULL),                   \
    };                                                                   \
    return trait(List(ErrorItem(Token(S)))).cons(item, NULL);            \
  }                                                                      \
                                                                         \
  static inline Hints(Token(S)) FUNC_NAME(merge, Hints(Token(S)))(       \
      Hints(Token(S)) hs1, Hints(Token(S)) hs2) {                        \
    ListT(ErrorItem(Token(S))) L = trait(List(ErrorItem(Token(S))));     \
    if (L.null(hs1)) {                                                   \
      return hs2;                                                        \
    }                                                                    \
    if (L.null(hs2)) {                                                   \
      return hs1;                                                        \
    }                                                                    \
    List(ErrorItem(Token(S))) x = NULL;                                  \
    while (!L.null(hs1)) {                                               \
      x = L.cons(L.head(hs1), x);                                        \
      hs1 = L.tail(hs1);                                                 \
    }                                                                    \
    while (!L.null(hs2)) {                                               \
      x = L.cons(L.head(hs2), x);                                        \
      hs2 = L.tail(hs2);                                                 \
    }                                                                    \
    L.reverse(&x);                                                       \
    return x;                                                            \
  }                                                                      \
                                                                         \
  static inline ParseError(S) FUNC_NAME(merge, ParseError(S))(           \
      ParseError(S) e1, ParseError(S) e2) {                              \
    if (e1.offset < e2.offset) {                                         \
      return e2;                                                         \
    }                                                                    \
    if (e1.offset > e2.offset) {                                         \
      return e1;                                                         \
    }                                                                    \
    ParseError(S) e;                                                     \
    e.offset = e1.offset;                                                \
    if (!e1.unexpected.none && !e2.unexpected.none) {                    \
      e.unexpected = e2.unexpected; /* TODO */                           \
    } else {                                                             \
      if (e1.unexpected.none) {                                          \
        e.unexpected = e2.unexpected;                                    \
      } else {                                                           \
        e.unexpected = e1.unexpected;                                    \
      }                                                                  \
    }                                                                    \
    e.expecting =                                                        \
        FUNC_NAME(merge, Hints(Token(S)))(e1.expecting, e2.expecting);   \
    return e;                                                            \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
// type alias for expecting items
#define Hints(T) List(ErrorItem(T))
