/* -*- coding: utf-8-unix -*- */
#pragma once

#include "ParsecFailure.h"

// -----------------------------------------------------------------------
#define ParsecChoice(...) TYPE_NAME(ParsecChoice, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecChoice(S, T)                                         \
  C_API_BEGIN                                                            \
                                                                         \
  trait_Eq(Parsec(S, T));                                                \
  trait_Ord(Parsec(S, T));                                               \
  trait_Mem(Parsec(S, T));                                               \
  trait_Array(Parsec(S, T));                                             \
                                                                         \
  typedef struct ParsecChoice(S, T) ParsecChoice(S, T);                  \
  struct ParsecChoice(S, T) {                                            \
    Parsec(S, T) (*empty)(void);                                         \
    Parsec(S, T) (*choice)(Array(Parsec(S, T)) ps);                      \
    Parsec(S, T) (*either)(Parsec(S, T) p1, Parsec(S, T) p2);            \
  };                                                                     \
                                                                         \
  ParsecChoice(S, T) Trait(ParsecChoice(S, T));                          \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecChoice(S, T)                                          \
  C_API_BEGIN                                                            \
                                                                         \
  static bool FUNC_NAME(eq, Eq(Parsec(S, T)))(Parsec(S, T) p1,           \
                                              Parsec(S, T) p2) {         \
    UNUSED(p1);                                                          \
    UNUSED(p2);                                                          \
    return true;                                                         \
  }                                                                      \
  static int FUNC_NAME(cmp, Ord(Parsec(S, T)))(Parsec(S, T) p1,          \
                                               Parsec(S, T) p2) {        \
    UNUSED(p1);                                                          \
    UNUSED(p2);                                                          \
    return 0;                                                            \
  }                                                                      \
  instance_Eq(Parsec(S, T), FUNC_NAME(eq, Eq(Parsec(S, T))));            \
  instance_Ord(Parsec(S, T), FUNC_NAME(cmp, Ord(Parsec(S, T))));         \
  impl_Mem(Parsec(S, T));                                                \
  impl_Array(Parsec(S, T));                                              \
                                                                         \
  impl_empty(S, T);                                                      \
  impl_choice(S, T);                                                     \
  impl_either(S, T);                                                     \
                                                                         \
  ParsecChoice(S, T) Trait(ParsecChoice(S, T)) {                         \
    return (ParsecChoice(S, T)){                                         \
        .empty = FUNC_NAME(empty, S, T),                                 \
        .choice = FUNC_NAME(choice, S, T),                               \
        .either = FUNC_NAME(either, S, T),                               \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_empty(S, T)                                                 \
  static Parsec(S, T) FUNC_NAME(empty, S, T)(void) {                     \
    return trait(ParsecFailure(S, T)).parseError((ParseError(S)){0});    \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_choice(S, T)                                                \
  static Parsec(S, T) FUNC_NAME(choice, S, T)(Array(Parsec(S, T)) ps) {  \
    ParsecChoice(S, T) C = trait(ParsecChoice(S, T));                    \
    ArrayT(Parsec(S, T)) A = trait(Array(Parsec(S, T)));                 \
    if (A.null(ps)) {                                                    \
      return C.empty();                                                  \
    }                                                                    \
    Parsec(S, T)* p = A.end(ps);                                         \
    Parsec(S, T) x = *(--p);                                             \
    while (p != A.begin(ps)) {                                           \
      x = C.either(*(--p), x);                                           \
    }                                                                    \
    return x;                                                            \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_either(S, T)                                                \
  typedef_Fn_r(                                                          \
      ContOk(S, T),  /* ok : underlying ok continuation */               \
      ParseError(S), /* e1 : an error caused by the 1st parser */        \
      ContOk(S, T)   /* x -> state -> hint -> reply */                   \
  );                                                                     \
  fn(FUNC_NAME(eitherOk, S, T),                                          \
     ContOk(S, T),    /* ok : underlying ok continuation */              \
     ParseError(S),   /* e1 : an error caused by the 1st parser */       \
     ContOkArgs(S, T) /* x -> state -> hint -> reply */                  \
  ) {                                                                    \
    g_bind((ok, e1, x, s, hs), *args);                                   \
    hs = FUNC_NAME(merge, Hints(Token(S)))(e1.expecting, hs);            \
    return fn_apply(ok, x, s, hs);                                       \
  }                                                                      \
                                                                         \
  typedef_Fn(ParseError(S), /* e1 : an error caused by the 1st parser */ \
             S,             /* s1 : state after the e1 occurred */       \
             ContErr(S, T)  /* error -> state -> reply */                \
  );                                                                     \
  typedef_Fn(ContErr(S, T), /* err : underlying error continuation */    \
             ParseError(S), /* e1 : an error caused by the 1st parser */ \
             S,             /* s1 : state after the e1 occurred */       \
             ContErr(S, T)  /* error -> state -> reply */                \
  );                                                                     \
  fn(FUNC_NAME(eitherErr, S, T),                                         \
     ContErr(S, T),    /* err : underlying error continuation */         \
     ParseError(S),    /* e1 : an error caused by the 1st parser */      \
     S,                /* s1 : state after the e1 occurred */            \
     ContErrArgs(S, T) /* error -> state -> reply */                     \
  ) {                                                                    \
    g_bind((err, e1, s1, e2, s2), *args);                                \
    ParseError(S) e = FUNC_NAME(merge, ParseError(S))(e1, e2);           \
    Stream(S) SS = trait(Stream(S));                                     \
    S s = (SS.offsetOf(s1) > SS.offsetOf(s2) ? s1 : s2);                 \
    return fn_apply(err, e, s);                                          \
  }                                                                      \
                                                                         \
  typedef_Fn_r(S,             /* s    : input state */                   \
               ContOk(S, T),  /* cok  : consumed ok */                   \
               ContErr(S, T), /* cerr : consumed error */                \
               ContOk(S, T),  /* eok  : empty ok */                      \
               ContErr(S, T), /* eerr : empty error */                   \
               Parsec(S, T),  /* p2   : the 2nd parser */                \
               ContErr(S, T)  /* error -> state -> reply */              \
  );                                                                     \
  fn(FUNC_NAME(eitherImpl2, S, T), S, /* s    : input state */           \
     ContOk(S, T),                    /* cok  : consumed ok */           \
     ContErr(S, T),                   /* cerr : consumed error */        \
     ContOk(S, T),                    /* eok  : empty ok */              \
     ContErr(S, T),                   /* eerr : empty error */           \
     Parsec(S, T),                    /* p2   : the 2nd parser */        \
     ContErrArgs(S, T)                /* error -> state -> reply */      \
  ) {                                                                    \
    g_bind((s, cok, cerr, eok, eerr, p2, e1, s1), *args);                \
    __auto_type f1 = FUNC_NAME(eitherErr, S, T)();                       \
    __auto_type f2 = FUNC_NAME(eitherOk, S, T)();                        \
    __auto_type f3 = FUNC_NAME(eitherErr, S, T)();                       \
    __auto_type cerr2 = fn_apply(f1, cerr, e1, s1);                      \
    __auto_type eok2 = fn_apply(f2, eok, e1);                            \
    __auto_type eerr2 = fn_apply(f3, eerr, e1, s1);                      \
    return fn_apply(p2.unParser, s, cok, cerr2, eok2, eerr2);            \
  }                                                                      \
                                                                         \
  typedef_Fn(                                                            \
      Parsec(S, T),  /* p1  : the 1st parser */                          \
      Parsec(S, T),  /* p2  : ther 2nd parser */                         \
      UnParser(S, T) /* s -> cok -> cerr -> eok -> eerr -> reply */      \
  );                                                                     \
  fn(FUNC_NAME(eitherImpl, S, T),                                        \
     Parsec(S, T),      /* p1  : the 1st parser */                       \
     Parsec(S, T),      /* p2  : ther 2nd parser */                      \
     UnParserArgs(S, T) /* s -> cok -> cerr -> eok -> eerr -> reply */   \
  ) {                                                                    \
    g_bind((p1, p2, s, cok, cerr, eok, eerr), *args);                    \
    __auto_type f = FUNC_NAME(eitherImpl2, S, T)();                      \
    __auto_type eerr2 = fn_apply(f, s, cok, cerr, eok, eerr, p2);        \
    return fn_apply(p1.unParser, s, cok, cerr, eok, eerr2);              \
  }                                                                      \
                                                                         \
  static Parsec(S, T)                                                    \
      FUNC_NAME(either, S, T)(Parsec(S, T) p1, Parsec(S, T) p2) {        \
    __auto_type f = FUNC_NAME(eitherImpl, S, T)();                       \
    return (Parsec(S, T)){                                               \
        .unParser = fn_apply(f, p1, p2),                                 \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
