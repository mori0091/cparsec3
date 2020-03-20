/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../../base/base_generics.h"

#include "../parsec.h"

// -----------------------------------------------------------------------
#define ParsecCombinator(...) TYPE_NAME(ParsecCombinator, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecCombinator(S, T)                                     \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ParsecCombinator(S, T) ParsecCombinator(S, T);          \
  struct ParsecCombinator(S, T) {                                        \
    Parsec(S, T) (*pLabel)(String l, Parsec(S, T) p);                    \
    Parsec(S, T) (*pHidden)(Parsec(S, T) p);                             \
    Parsec(S, T) (*pTryp)(Parsec(S, T) p);                               \
    Parsec(S, T) (*pLookAhead)(Parsec(S, T) p);                          \
    Parsec(S, None) (*pNotFollowedBy)(Parsec(S, T) p);                   \
  };                                                                     \
                                                                         \
  ParsecCombinator(S, T) Trait(ParsecCombinator(S, T));                  \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecCombinator(S, T)                                      \
  C_API_BEGIN                                                            \
                                                                         \
  typedef_Fn(Parsec(S, T), UnParser(S, T));                              \
  typedef_Fn(String, Parsec(S, T), UnParser(S, T));                      \
                                                                         \
  impl_label(S, T);                                                      \
  impl_tryp(S, T);                                                       \
                                                                         \
  ParsecCombinator(S, T) Trait(ParsecCombinator(S, T)) {                 \
    return (ParsecCombinator(S, T)){                                     \
        .pLabel = FUNC_NAME(label, S, T),                                \
        .pHidden = 0,                                                    \
        .pTryp = FUNC_NAME(tryp, S, T),                                  \
        .pLookAhead = 0,                                                 \
        .pNotFollowedBy = 0,                                             \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* label(l, p) */
#define impl_label(S, T)                                                 \
  typedef_Fn_r(String, ContErr(S, T), ContErr(S, T));                    \
  fn(FUNC_NAME(contErrLabel, S, T), /* name */                           \
     String,                        /* l : a label string*/              \
     ContErr(S, T),                 /* f : underlying continuation */    \
     ContErrArgs(S, T)              /* error -> state -> reply */        \
  ) {                                                                    \
    g_bind((l, f, e, st), *args);                                        \
    ErrorItem(S) item = {.type = LABEL, .label = l};                     \
    e.expecting = g_list(ErrorItem(S), item);                            \
    return fn_apply(f, e, st);                                           \
  }                                                                      \
                                                                         \
  fn(FUNC_NAME(labelImpl, S, T), /* name */                              \
     String,                     /* l : a label string */                \
     Parsec(S, T),               /* p : underlying parser */             \
     UnParserArgs(S, T) /* st -> cok -> cerr -> eok -> eerr -> reply */  \
  ) {                                                                    \
    g_bind((l, p, st, cok, cerr, eok, eerr), *args);                     \
    __auto_type f = FUNC_NAME(contErrLabel, S, T)();                     \
    __auto_type eerr_ = fn_apply(f, l, eerr);                            \
    return fn_apply(p.unParser, st, cok, cerr, eok, eerr_);              \
  }                                                                      \
                                                                         \
  static Parsec(S, T) FUNC_NAME(label, S, T)(String l, Parsec(S, T) p) { \
    __auto_type f = FUNC_NAME(labelImpl, S, T)();                        \
    return (Parsec(S, T)){.unParser = fn_apply(f, l, p)};                \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
/* tryp(p) */
#define impl_tryp(S, T)                                                  \
  typedef_Fn_r(ContErr(S, T), S, ContErr(S, T));                         \
  fn(FUNC_NAME(contErrBackTrack, S, T), /* name */                       \
     ContErr(S, T),    /* f : underlying continuation */                 \
     S,                /* s : state that overrides the given state */    \
     ContErrArgs(S, T) /* error -> state -> reply */                     \
  ) {                                                                    \
    g_bind((f, s, e), *args);                                            \
    return fn_apply(f, e, s);                                            \
  }                                                                      \
                                                                         \
  fn(FUNC_NAME(trypImpl, S, T), /* name */                               \
     Parsec(S, T),              /* p : underlying parser */              \
     UnParserArgs(S,                                                     \
                  T) /* state -> cok -> cerr -> eok -> eerr -> reply */  \
  ) {                                                                    \
    g_bind((p, s, cok, , eok, eerr), *args);                             \
    __auto_type f = FUNC_NAME(contErrBackTrack, S, T)();                 \
    __auto_type eerr_ = fn_apply(f, eerr, s);                            \
    return fn_apply(p.unParser, s, cok, eerr_, eok, eerr_);              \
  }                                                                      \
                                                                         \
  static Parsec(S, T) FUNC_NAME(tryp, S, T)(Parsec(S, T) p) {            \
    __auto_type f = FUNC_NAME(trypImpl, S, T)();                         \
    return (Parsec(S, T)){.unParser = fn_apply(f, p)};                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
