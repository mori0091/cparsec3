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
  trait_Eq(ErrorItem(T));                                                \
  trait_Ord(ErrorItem(T));                                               \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define impl_ErrorItem(T)                                                \
  C_API_BEGIN                                                            \
                                                                         \
  static bool FUNC_NAME(eq, Eq(ErrorItem(T)))(ErrorItem(T) a,            \
                                              ErrorItem(T) b) {          \
    if (a.type != b.type) {                                              \
      return false;                                                      \
    }                                                                    \
    switch (a.type) {                                                    \
    case LABEL:                                                          \
      return trait(Eq(String)).eq(a.label, b.label);                     \
    case TOKENS:                                                         \
      return trait(Eq(List(T))).eq(a.tokens, b.tokens);                  \
    case END_OF_INPUT:                                                   \
      return true;                                                       \
    default:                                                             \
      return false;                                                      \
    }                                                                    \
  }                                                                      \
                                                                         \
  instance_Eq(ErrorItem(T), FUNC_NAME(eq, Eq(ErrorItem(T))));            \
                                                                         \
  static int FUNC_NAME(cmp, Ord(ErrorItem(T)))(ErrorItem(T) a,           \
                                               ErrorItem(T) b) {         \
    int x = trait(Ord(int)).cmp(a.type, b.type);                         \
    if (x) {                                                             \
      return x;                                                          \
    }                                                                    \
    switch (a.type) {                                                    \
    case LABEL:                                                          \
      return trait(Ord(String)).cmp(a.label, b.label);                   \
    case TOKENS:                                                         \
      return trait(Ord(List(T))).cmp(a.tokens, b.tokens);                \
    case END_OF_INPUT:                                                   \
    default:                                                             \
      return 0;                                                          \
    }                                                                    \
  }                                                                      \
                                                                         \
  instance_Ord(ErrorItem(T), FUNC_NAME(cmp, Ord(ErrorItem(T))));         \
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
  static inline void FUNC_NAME(print, ErrorItem(T))(ErrorItem(T) e) {    \
    switch (e.type) {                                                    \
    case LABEL:                                                          \
      printf("%s", e.label);                                             \
      break;                                                             \
    case TOKENS:;                                                        \
      ListT(T) L = trait(List(T));                                       \
      List(T) xs = e.tokens;                                             \
      if (!xs) {                                                         \
        break;                                                           \
      }                                                                  \
      printf("'%c'", L.head(xs));                                        \
      xs = L.tail(xs);                                                   \
      if (!xs) {                                                         \
        break;                                                           \
      }                                                                  \
      if (!L.tail(xs)) {                                                 \
        printf(" or '%c'", L.head(xs));                                  \
        break;                                                           \
      }                                                                  \
      for (; L.tail(xs); xs = L.tail(xs)) {                              \
        printf(", '%c'", L.head(xs));                                    \
      }                                                                  \
      printf(" or '%c'", L.head(xs));                                    \
      break;                                                             \
    case END_OF_INPUT:                                                   \
      printf("end of input");                                            \
      break;                                                             \
    default:                                                             \
      break;                                                             \
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
  trait_Eq(ParseError(S));                                               \
  trait_Ord(ParseError(S));                                              \
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
  static inline void FUNC_NAME(print, ParseError(S))(ParseError(S) e) {  \
    printf("error:%" PRIuMAX ":\n", e.offset);                           \
    if (FUNC_NAME(isUnknown, ParseError(S))(e)) {                        \
      printf("  unknown error\n");                                       \
      return;                                                            \
    }                                                                    \
    if (!e.unexpected.none) {                                            \
      printf("  unexpected ");                                           \
      FUNC_NAME(print, ErrorItem(Token(S)))(e.unexpected.value);         \
      printf("\n");                                                      \
    }                                                                    \
    if (e.expecting) {                                                   \
      printf("  expecting ");                                            \
      ListT(ErrorItem(Token(S))) L = trait(List(ErrorItem(Token(S))));   \
      List(ErrorItem(Token(S))) xs = e.expecting;                        \
      for (; xs; xs = L.tail(xs)) {                                      \
        FUNC_NAME(print, ErrorItem(Token(S)))(L.head(xs));               \
      }                                                                  \
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
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
// type alias for expecting items
#define Hints(T) List(ErrorItem(T))

// -----------------------------------------------------------------------
#define ParseErrorBundle(S) TYPE_NAME(ParseErrorBundle, S)

// -----------------------------------------------------------------------
#define trait_ParseErrorBundle(S)                                        \
  C_API_BEGIN                                                            \
                                                                         \
  trait_ParseError(S);                                                   \
  trait_List(ParseError(S));                                             \
                                                                         \
  typedef struct ParseErrorBundle(S) ParseErrorBundle(S);                \
  struct ParseErrorBundle(S) {                                           \
    List(ParseError(S)) errors;                                          \
    PosState(S) posState;                                                \
  };                                                                     \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParseErrorBundle(S)                                         \
  C_API_BEGIN                                                            \
                                                                         \
  impl_List(ParseError(S));                                              \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
