/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
// ParsecRunner(S, T)

#define TRAIT_PARSECRUNNER(S, T) trait(ParsecRunner(S, T))

#define GENERIC_PARSECRUNNER(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECRUNNER, BIND(S, PARSER_RETURN_TYPES(S)))

#define parseTest(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p)                           \
      .parseTest(p, CPARSEC_STREAM_NEW(input))

#define runParsec(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p).runParsec(p, input)

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
  if (!TMPID.result.success) {                                           \
    __auto_type _err_ = (TMPID.consumed ? _cerr_ : _eerr_);              \
    return fn_apply(_err_, TMPID.result.err, TMPID.result.state);        \
  }                                                                      \
  _s_ = TMPID.result.state;

#define SCAN2(_p_, _x_)                                                  \
  SCAN1(_p_);                                                            \
  __auto_type _x_ = TMPID.result.ok;

#define RETURN(_x_)                                                      \
  do {                                                                   \
    Stream(CPARSEC_STREAM_TYPE) SS = trait(Stream(CPARSEC_STREAM_TYPE)); \
    __auto_type _ok_ =                                                   \
        (SS.offsetOf(_s0_) < SS.offsetOf(_s_) ? _cok_ : _eok_);          \
    Hints(Token(CPARSEC_STREAM_TYPE)) empty_hints = {0};                 \
    return fn_apply(_ok_, _x_, _s_, empty_hints);                        \
  } while (0)
