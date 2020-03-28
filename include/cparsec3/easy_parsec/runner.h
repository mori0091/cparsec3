/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base_generics.h"

#include "config.h"
#include "types.h"

#include "../parsec/parsec.h"
#include "../parsec/stream.h"

#include "../parsec/ParsecRunner.h"

// -----------------------------------------------------------------------
// ParsecRunner(S, T)

#define TRAIT_PARSECRUNNER(S, T) trait(ParsecRunner(S, T))

#define GENERIC_PARSECRUNNER(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECRUNNER, BIND(S, PARSER_RETURN_TYPES(S)))

#define runParsec(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p).pRunParsec(p, input)

#define runParser(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p).pRunParser(p, input)

#define parseTest(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p)                           \
      .pParseTest(p, CPARSEC_STREAM_NEW(input))

// -----------------------------------------------------------------------
trait_ParseError(CPARSEC_STREAM_TYPE);
BIND_FOR(trait_ParsecRunner, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));

#ifdef CPARSEC_CONFIG_IMPLEMENT
impl_ParseError(CPARSEC_STREAM_TYPE);
BIND_FOR(impl_ParsecRunner, CPARSEC_STREAM_TYPE,
         PARSER_RETURN_TYPES(CPARSEC_STREAM_TYPE));
#endif

// -----------------------------------------------------------------------
#define PARSER(R) Parsec(CPARSEC_STREAM_TYPE, R)
#define FnParser(R) UnParser(CPARSEC_STREAM_TYPE, R)
#define FnParserArgs(R) UnParserArgs(CPARSEC_STREAM_TYPE, R)

#define typedef_FnParser(...)                                            \
  CAT(typedef_FnParser, VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)

#define typedef_FnParser1(R) typedef_Fn(FnParser(R))
#define typedef_FnParser2(T1, R) typedef_Fn(T1, FnParser(R))
#define typedef_FnParser3(T1, T2, R) typedef_Fn(T1, T2, FnParser(R))
#define typedef_FnParser4(T1, T2, T3, R)                                 \
  typedef_Fn(T1, T2, T3, FnParser(R))

#define parsec(name, ...)                                                \
  CAT(parsec, VARIADIC_SIZE(__VA_ARGS__))(name, __VA_ARGS__)

#define parsec1(name, R)                                                 \
  static FnParser(R) FUNC_NAME(name, PARSER(R))(void);                   \
  PARSER(R) name(void) {                                                 \
    return (PARSER(R)){                                                  \
        FUNC_NAME(name, PARSER(R))(),                                    \
    };                                                                   \
  }                                                                      \
  fn(FUNC_NAME(name, PARSER(R)), FnParserArgs(R))

#define parsec2(name, T1, R)                                             \
  static Fn(T1, FnParser(R)) FUNC_NAME(name, PARSER(R))(void);           \
  PARSER(R) name(T1 x1) {                                                \
    Fn(T1, FnParser(R)) f = FUNC_NAME(name, PARSER(R))();                \
    return (PARSER(R)){fn_apply(f, x1)};                                 \
  }                                                                      \
  fn(FUNC_NAME(name, PARSER(R)), T1, FnParserArgs(R))

#define parsec3(name, T1, T2, R)                                         \
  static Fn(T1, T2, FnParser(R)) FUNC_NAME(name, PARSER(R))(void);       \
  PARSER(R) name(T1 x1, T2 x2) {                                         \
    Fn(T1, T2, FnParser(R)) f = FUNC_NAME(name, PARSER(R))();            \
    return (PARSER(R)){fn_apply(f, x1, x2)};                             \
  }                                                                      \
  fn(FUNC_NAME(name, PARSER(R)), T1, T2, FnParserArgs(R))

#define parsec4(name, T1, T2, T3, R)                                     \
  static Fn(T1, T2, T3, FnParser(R)) FUNC_NAME(name, PARSER(R))(void);   \
  PARSER(R) name(T1 x1, T2 x2, T3 x3) {                                  \
    Fn(T1, T2, T3, FnParser(R)) f = FUNC_NAME(name, PARSER(R))();        \
    return (PARSER(R)){fn_apply(f, x1, x2, x3)};                         \
  }                                                                      \
  fn(FUNC_NAME(name, PARSER(R)), T1, T2, T3, FnParserArgs(R))

// -----------------------------------------------------------------------
#define DO()                                                             \
  g_bind((_s0_, _cok_, _cerr_, _eok_, _eerr_), *args);                   \
  __auto_type _s_ = _s0_;

#define DO_WITH(...)                                                     \
  g_bind((__VA_ARGS__, _s0_, _cok_, _cerr_, _eok_, _eerr_), *args);      \
  __auto_type _s_ = _s0_;

#define SCAN(...) CAT(SCAN, VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)

#define SCAN1(_p_)                                                       \
  __auto_type TMPID = runParsec(_p_, _s_);                               \
  _s_ = TMPID.result.state;                                              \
  do {                                                                   \
    if (!TMPID.result.success) {                                         \
      Stream(CPARSEC_STREAM_TYPE) SS =                                   \
          trait(Stream(CPARSEC_STREAM_TYPE));                            \
      __auto_type _err_ =                                                \
          (SS.offsetOf(_s0_) < SS.offsetOf(_s_) ? _cerr_ : _eerr_);      \
      return fn_apply(_err_, TMPID.result.err, _s_);                     \
    }                                                                    \
  } while (0)

#define SCAN2(_p_, _x_)                                                  \
  SCAN1(_p_);                                                            \
  __auto_type _x_ = TMPID.result.ok;

#define FAIL(_msg_)                                                      \
  do {                                                                   \
    Stream(CPARSEC_STREAM_TYPE) SS = trait(Stream(CPARSEC_STREAM_TYPE)); \
    __auto_type _err_ =                                                  \
        (SS.offsetOf(_s0_) < SS.offsetOf(_s_) ? _cerr_ : _eerr_);        \
    Hints(CPARSEC_STREAM_TYPE) empty_hints = {0};                        \
    ParseError(CPARSEC_STREAM_TYPE) e =                                  \
        trait(ParseError(CPARSEC_STREAM_TYPE))                           \
            .unexpected_label(SS.offsetOf(_s_), _msg_, empty_hints);     \
    return fn_apply(_err_, e, _s_);                                      \
  } while (0)

#define RETURN(_x_)                                                      \
  do {                                                                   \
    Stream(CPARSEC_STREAM_TYPE) SS = trait(Stream(CPARSEC_STREAM_TYPE)); \
    __auto_type _ok_ =                                                   \
        (SS.offsetOf(_s0_) < SS.offsetOf(_s_) ? _cok_ : _eok_);          \
    Hints(CPARSEC_STREAM_TYPE) empty_hints = {0};                        \
    return fn_apply(_ok_, _x_, _s_, empty_hints);                        \
  } while (0)
