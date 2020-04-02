/* -*- coding: utf-8-unix -*- */
#pragma once

// -----------------------------------------------------------------------
#define PARSER(R) Parsec(CPARSEC_STREAM_TYPE, R)

#define ParsecEnv(...) TYPE_NAME(ParsecEnv, __VA_ARGS__)

#define parsec(name, ...)                                                \
  def_parsec(CPARSEC_STREAM_TYPE, name, __VA_ARGS__)

#define def_parsec(S, name, ...)                                         \
  CAT(def_parsec, VARIADIC_SIZE(__VA_ARGS__))(S, name, __VA_ARGS__)

#define def_parsec1(S, name, R)                                          \
  typedef struct {                                                       \
    struct {                                                             \
      Stream(S) stream;                                                  \
      ParseErrorT(S) parse_error;                                        \
      ErrorItemT(S) error_item;                                          \
    } traits;                                                            \
  } ParsecEnv(name);                                                     \
  typedef_Fn(ParsecEnv(name), UnParser(S, R));                           \
  static Fn(ParsecEnv(name), UnParser(S, R))                             \
      FUNC_NAME(impl, name)(void);                                       \
  Parsec(S, R) name(void) {                                              \
    Fn(ParsecEnv(name), UnParser(S, R)) f = FUNC_NAME(impl, name)();     \
    ParsecEnv(name) par = {                                              \
        .traits.stream = trait(Stream(S)),                               \
        .traits.parse_error = trait(ParseError(S)),                      \
        .traits.error_item = trait(ErrorItem(S)),                        \
    };                                                                   \
    return (Parsec(S, R)){fn_apply(f, par)};                             \
  }                                                                      \
  fn(FUNC_NAME(impl, name), ParsecEnv(name), UnParserArgs(S, R))

#define def_parsec2(S, name, T1, R)                                      \
  typedef struct {                                                       \
    struct {                                                             \
      Stream(S) stream;                                                  \
      ParseErrorT(S) parse_error;                                        \
      ErrorItemT(S) error_item;                                          \
    } traits;                                                            \
    T1 e1;                                                               \
  } ParsecEnv(name);                                                     \
  typedef_Fn(ParsecEnv(name), UnParser(S, R));                           \
  static Fn(ParsecEnv(name), UnParser(S, R))                             \
      FUNC_NAME(impl, name)(void);                                       \
  Parsec(S, R) name(T1 x1) {                                             \
    Fn(ParsecEnv(name), UnParser(S, R)) f = FUNC_NAME(impl, name)();     \
    ParsecEnv(name) par = {                                              \
        .traits.stream = trait(Stream(S)),                               \
        .traits.parse_error = trait(ParseError(S)),                      \
        .traits.error_item = trait(ErrorItem(S)),                        \
        .e1 = x1,                                                        \
    };                                                                   \
    return (Parsec(S, R)){fn_apply(f, par)};                             \
  }                                                                      \
  fn(FUNC_NAME(impl, name), ParsecEnv(name), UnParserArgs(S, R))

#define def_parsec3(S, name, T1, T2, R)                                  \
  typedef struct {                                                       \
    struct {                                                             \
      Stream(S) stream;                                                  \
      ParseErrorT(S) parse_error;                                        \
      ErrorItemT(S) error_item;                                          \
    } traits;                                                            \
    T1 e1;                                                               \
    T2 e2;                                                               \
  } ParsecEnv(name);                                                     \
  typedef_Fn(ParsecEnv(name), UnParser(S, R));                           \
  static Fn(ParsecEnv(name), UnParser(S, R))                             \
      FUNC_NAME(impl, name)(void);                                       \
  Parsec(S, R) name(T1 x1, T2 x2) {                                      \
    Fn(ParsecEnv(name), UnParser(S, R)) f = FUNC_NAME(impl, name)();     \
    ParsecEnv(name) par = {                                              \
        .traits.stream = trait(Stream(S)),                               \
        .traits.parse_error = trait(ParseError(S)),                      \
        .traits.error_item = trait(ErrorItem(S)),                        \
        .e1 = x1,                                                        \
        .e2 = x2,                                                        \
    };                                                                   \
    return (Parsec(S, R)){fn_apply(f, par)};                             \
  }                                                                      \
  fn(FUNC_NAME(impl, name), ParsecEnv(name), UnParserArgs(S, R))

#define def_parsec4(S, name, T1, T2, T3, R)                              \
  typedef struct {                                                       \
    struct {                                                             \
      Stream(S) stream;                                                  \
      ParseErrorT(S) parse_error;                                        \
      ErrorItemT(S) error_item;                                          \
    } traits;                                                            \
    T1 e1;                                                               \
    T2 e2;                                                               \
    T3 e3;                                                               \
  } ParsecEnv(name);                                                     \
  typedef_Fn(ParsecEnv(name), UnParser(S, R));                           \
  static Fn(ParsecEnv(name), UnParser(S, R))                             \
      FUNC_NAME(impl, name)(void);                                       \
  Parsec(S, R) name(T1 x1, T2 x2, T3 x3) {                               \
    Fn(ParsecEnv(name), UnParser(S, R)) f = FUNC_NAME(impl, name)();     \
    ParsecEnv(name) par = {                                              \
        .traits.stream = trait(Stream(S)),                               \
        .traits.parse_error = trait(ParseError(S)),                      \
        .traits.error_item = trait(ErrorItem(S)),                        \
        .e1 = x1,                                                        \
        .e2 = x2,                                                        \
        .e3 = x3,                                                        \
    };                                                                   \
    return (Parsec(S, R)){fn_apply(f, par)};                             \
  }                                                                      \
  fn(FUNC_NAME(impl, name), ParsecEnv(name), UnParserArgs(S, R))

// -----------------------------------------------------------------------
#define DO()                                                             \
  g_bind((_par_, _s0_, _cok_, _cerr_, _eok_, _eerr_), *args);            \
  __auto_type _s_ = _s0_;

#define WITH(...) g_bind((__VA_ARGS__), _par_);

#define DO_WITH(...) DO() WITH(__VA_ARGS__)

#define SCAN(...) CAT(SCAN, VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)

#define SCAN1(_p_)                                                       \
  __auto_type TMPID = runParsec(_p_, _s_);                               \
  _s_ = TMPID.result.state;                                              \
  do {                                                                   \
    if (!TMPID.result.success) {                                         \
      __auto_type SS = _par_.traits.stream;                              \
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
    __auto_type SS = _par_.traits.stream;                                \
    __auto_type PE = _par_.traits.parse_error;                           \
    __auto_type EI = _par_.traits.error_item;                            \
    __auto_type _err_ =                                                  \
        (SS.offsetOf(_s0_) < SS.offsetOf(_s_) ? _cerr_ : _eerr_);        \
    return fn_apply(                                                     \
        _err_,                                                           \
        PE.unexpected_label(SS.offsetOf(_s_), _msg_, EI.emptyHints()),   \
        _s_);                                                            \
  } while (0)

#define RETURN(_x_)                                                      \
  do {                                                                   \
    __auto_type SS = _par_.traits.stream;                                \
    __auto_type EI = _par_.traits.error_item;                            \
    __auto_type _ok_ =                                                   \
        (SS.offsetOf(_s0_) < SS.offsetOf(_s_) ? _cok_ : _eok_);          \
    return fn_apply(_ok_, _x_, _s_, EI.emptyHints());                    \
  } while (0)
