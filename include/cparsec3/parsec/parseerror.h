/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

// -----------------------------------------------------------------------
#define ErrorItem(S) TYPE_NAME(ErrorItem, S)   // data type
#define ErrorItemT(S) TYPE_NAME(ErrorItemT, S) // trait

// -----------------------------------------------------------------------
// type alias for expecting items
#define Hints(S) List(ErrorItem(S)) // data type

// -----------------------------------------------------------------------
#define trait_ErrorItem(S)                                               \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ErrorItem(S) ErrorItem(S);                              \
  struct ErrorItem(S) {                                                  \
    enum {                                                               \
      LABEL,                                                             \
      TOKENS,                                                            \
      END_OF_INPUT,                                                      \
    } type;                                                              \
    union {                                                              \
      String label;                                                      \
      List(Token(S)) tokens;                                             \
    };                                                                   \
  };                                                                     \
                                                                         \
  trait_Maybe(ErrorItem(S));                                             \
  typedef ErrorItem(S) Item(List(ErrorItem(S)));                         \
  trait_List(ErrorItem(S));                                              \
                                                                         \
  typedef struct ErrorItemT(S) ErrorItemT(S);                            \
  struct ErrorItemT(S) {                                                 \
    bool (*null)(ErrorItem(S) e);                                        \
    Hints(S) (*toHints)(Token(S) t);                                     \
    Hints(S) (*merge)(Hints(S) hs1, Hints(S) hs2);                       \
    void (*print)(ErrorItem(S) e);                                       \
  };                                                                     \
                                                                         \
  ErrorItemT(S) Trait(ErrorItem(S));                                     \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

#define impl_ErrorItem(S)                                                \
  C_API_BEGIN                                                            \
                                                                         \
  /* impl_Maybe(ErrorItem(S)); */                                        \
  impl_List(ErrorItem(S));                                               \
                                                                         \
  static inline bool FUNC_NAME(null, ErrorItem(S))(ErrorItem(S) e) {     \
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
  static inline void FUNC_NAME(print, ErrorItem(S))(ErrorItem(S) e) {    \
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
                                                                         \
  static inline Hints(S) FUNC_NAME(toHints, Hints(S))(Token(S) t) {      \
    ErrorItem(S) item = {                                                \
        .type = TOKENS,                                                  \
        .tokens = trait(List(Token(S))).cons(t, NULL),                   \
    };                                                                   \
    return trait(List(ErrorItem(S))).cons(item, NULL);                   \
  }                                                                      \
                                                                         \
  static inline Hints(S)                                                 \
      FUNC_NAME(merge, Hints(S))(Hints(S) hs1, Hints(S) hs2) {           \
    ListT(ErrorItem(S)) L = trait(List(ErrorItem(S)));                   \
    if (L.null(hs1)) {                                                   \
      return hs2;                                                        \
    }                                                                    \
    if (L.null(hs2)) {                                                   \
      return hs1;                                                        \
    }                                                                    \
    List(ErrorItem(S)) x = NULL;                                         \
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
  ErrorItemT(S) Trait(ErrorItem(S)) {                                    \
    return (ErrorItemT(S)){                                              \
        .null = FUNC_NAME(null, ErrorItem(S)),                           \
        .toHints = FUNC_NAME(toHints, Hints(S)),                         \
        .merge = FUNC_NAME(merge, Hints(S)),                             \
        .print = FUNC_NAME(print, ErrorItem(S)),                         \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define ParseError(S) TYPE_NAME(ParseError, S)
#define ParseErrorT(S) TYPE_NAME(ParseErrorT, S)

// -----------------------------------------------------------------------
#define trait_ParseError(S)                                              \
  C_API_BEGIN                                                            \
                                                                         \
  trait_ErrorItem(S);                                                    \
                                                                         \
  typedef struct ParseError(S) ParseError(S);                            \
  struct ParseError(S) {                                                 \
    Offset offset;                                                       \
    Maybe(ErrorItem(S)) unexpected;                                      \
    List(ErrorItem(S)) expecting;                                        \
  };                                                                     \
                                                                         \
  typedef struct ParseErrorT(S) ParseErrorT(S);                          \
  struct ParseErrorT(S) {                                                \
    bool (*null)(ParseError(S) e);                                       \
    ParseError(S) (*unexpected)(Offset o, ErrorItem(S) e, Hints(S) hs);  \
    ParseError(S) (*unexpected_end_of_input)(Offset o, Hints(S) hs);     \
    ParseError(S) (*unexpected_token)(Offset o, Token(S) t,              \
                                      Hints(S) hs);                      \
    ParseError(S) (*unexpected_tokens)(Offset o, Tokens(S) chk,          \
                                       Hints(S) hs);                     \
    ParseError(S) (*unexpected_label)(Offset o, String l, Hints(S) hs);  \
    ParseError(S) (*merge)(ParseError(S) e1, ParseError(S) e2);          \
    void (*print)(ParseError(S) e);                                      \
  };                                                                     \
                                                                         \
  ParseErrorT(S) Trait(ParseError(S));                                   \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParseError(S)                                               \
  C_API_BEGIN                                                            \
                                                                         \
  impl_ErrorItem(S);                                                     \
                                                                         \
  static inline bool FUNC_NAME(null, ParseError(S))(ParseError(S) e) {   \
    ErrorItemT(S) EI = trait(ErrorItem(S));                              \
    ListT(ErrorItem(S)) L = trait(List(ErrorItem(S)));                   \
    return ((e.unexpected.none || EI.null(e.unexpected.value)) &&        \
            L.null(e.expecting));                                        \
  }                                                                      \
                                                                         \
  static inline ParseError(S) FUNC_NAME(unexpected, ParseError(S))(      \
      Offset o, ErrorItem(S) i, Hints(S) hs) {                           \
    return (ParseError(S)){                                              \
        .offset = o,                                                     \
        .unexpected.value = i,                                           \
        .expecting = hs,                                                 \
    };                                                                   \
  }                                                                      \
                                                                         \
  static inline ParseError(S) FUNC_NAME(                                 \
      unexpected_end_of_input, ParseError(S))(Offset o, Hints(S) hs) {   \
    return (ParseError(S)){                                              \
        .offset = o,                                                     \
        .unexpected.value.type = END_OF_INPUT,                           \
        .expecting = hs,                                                 \
    };                                                                   \
  }                                                                      \
                                                                         \
  static inline ParseError(S)                                            \
      FUNC_NAME(unexpected_token, ParseError(S))(Offset o, Token(S) t,   \
                                                 Hints(S) hs) {          \
    return (ParseError(S)){                                              \
        .offset = o,                                                     \
        .unexpected.value.type = TOKENS,                                 \
        .unexpected.value.tokens = trait(List(Token(S))).cons(t, NULL),  \
        .expecting = hs,                                                 \
    };                                                                   \
  }                                                                      \
                                                                         \
  static inline ParseError(S)                                            \
      FUNC_NAME(unexpected_tokens,                                       \
                ParseError(S))(Offset o, Tokens(S) chk, Hints(S) hs) {   \
    return (ParseError(S)){                                              \
        .offset = o,                                                     \
        .unexpected.value.type = TOKENS,                                 \
        .unexpected.value.tokens = trait(Stream(S)).chunkToTokens(chk),  \
        .expecting = hs,                                                 \
    };                                                                   \
  }                                                                      \
                                                                         \
  static inline ParseError(S)                                            \
      FUNC_NAME(unexpected_label, ParseError(S))(Offset o, String l,     \
                                                 Hints(S) hs) {          \
    return (ParseError(S)){                                              \
        .offset = o,                                                     \
        .unexpected.value.type = LABEL,                                  \
        .unexpected.value.label = l,                                     \
        .expecting = hs,                                                 \
    };                                                                   \
  }                                                                      \
                                                                         \
  static inline void FUNC_NAME(print, ParseError(S))(ParseError(S) e) {  \
    ErrorItemT(S) EI = trait(ErrorItem(S));                              \
    ParseErrorT(S) E = trait(ParseError(S));                             \
    /* printf("error:%" PRIdMAX ":\n", e.offset); */                     \
    if (E.null(e)) {                                                     \
      printf("unknown error\n");                                         \
      return;                                                            \
    }                                                                    \
    if (!e.unexpected.none) {                                            \
      printf("unexpected ");                                             \
      EI.print(e.unexpected.value);                                      \
      printf("\n");                                                      \
    }                                                                    \
    if (e.expecting) {                                                   \
      printf("expecting ");                                              \
      ListT(ErrorItem(S)) L = trait(List(ErrorItem(S)));                 \
      List(ErrorItem(S)) xs = e.expecting;                               \
      if (!xs) {                                                         \
        printf("\n");                                                    \
        return;                                                          \
      }                                                                  \
      EI.print(L.head(xs));                                              \
      xs = L.tail(xs);                                                   \
      if (!xs) {                                                         \
        printf("\n");                                                    \
        return;                                                          \
      }                                                                  \
      if (!L.tail(xs)) {                                                 \
        printf(" or ");                                                  \
        EI.print(L.head(xs));                                            \
        printf("\n");                                                    \
        return;                                                          \
      }                                                                  \
      for (; L.tail(xs); xs = L.tail(xs)) {                              \
        printf(", ");                                                    \
        EI.print(L.head(xs));                                            \
      }                                                                  \
      printf(", or ");                                                   \
      EI.print(L.head(xs));                                              \
      printf("\n");                                                      \
    }                                                                    \
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
    e.expecting = trait(ErrorItem(S)).merge(e1.expecting, e2.expecting); \
    return e;                                                            \
  }                                                                      \
                                                                         \
  ParseErrorT(S) Trait(ParseError(S)) {                                  \
    return (ParseErrorT(S)){                                             \
        .null = FUNC_NAME(null, ParseError(S)),                          \
        .unexpected = FUNC_NAME(unexpected, ParseError(S)),              \
        .unexpected_end_of_input =                                       \
            FUNC_NAME(unexpected_end_of_input, ParseError(S)),           \
        .unexpected_token = FUNC_NAME(unexpected_token, ParseError(S)),  \
        .unexpected_tokens =                                             \
            FUNC_NAME(unexpected_tokens, ParseError(S)),                 \
        .unexpected_label = FUNC_NAME(unexpected_label, ParseError(S)),  \
        .merge = FUNC_NAME(merge, ParseError(S)),                        \
        .print = FUNC_NAME(print, ParseError(S)),                        \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS
