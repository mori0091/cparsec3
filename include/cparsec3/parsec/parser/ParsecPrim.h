/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../../base/base_generics.h"

#include "../parsec.h"

// -----------------------------------------------------------------------
#define ParsecPrim(...) TYPE_NAME(ParsecPrim, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecPrim(S, T)                                           \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ParsecPrim(S, T) ParsecPrim(S, T);                      \
  struct ParsecPrim(S, T) {                                              \
    Parsec(S, T) (*label)(String l, Parsec(S, T) p);                     \
    Parsec(S, T) (*hidden)(Parsec(S, T) p);                              \
    Parsec(S, T) (*tryp)(Parsec(S, T) p);                                \
    Parsec(S, T) (*lookAhead)(Parsec(S, T) p);                           \
    Parsec(S, None) (*notFollowedBy)(Parsec(S, T) p);                    \
  };                                                                     \
                                                                         \
  ParsecPrim(S, T) Trait(ParsecPrim(S, T));                              \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecPrim(S, T)                                            \
  C_API_BEGIN                                                            \
                                                                         \
  typedef_Fn(Parsec(S, T), UnParser(S, T));                              \
  typedef_Fn(String, Parsec(S, T), UnParser(S, T));                      \
                                                                         \
  impl_label(S, T);                                                      \
  impl_tryp(S, T);                                                       \
                                                                         \
  ParsecPrim(S, T) Trait(ParsecPrim(S, T)) {                             \
    return (ParsecPrim(S, T)){                                           \
        .label = FUNC_NAME(label, S, T),                                 \
        .hidden = 0,                                                     \
        .tryp = FUNC_NAME(tryp, S, T),                                   \
        .lookAhead = 0,                                                  \
        .notFollowedBy = 0,                                              \
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
    ErrorItem(Token(S)) item = {.type = LABEL, .label = l};              \
    e.expecting = g_list(ErrorItem(Token(S)), item);                     \
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
