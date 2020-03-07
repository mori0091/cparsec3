/* -*- coding: utf-8-unix -*- */
// #include <cparsec3/base/base.h>
#include <cparsec3/base/base_generics.h>

#include <cparsec3/parsec/state.h>
#include <cparsec3/parsec/stream.h>

#include <cparsec3/parsec/ParsecBase.h>
#include <cparsec3/parsec/ParsecRunner.h>

#include <cparsec3/parsec/ParsecFailure.h>
#include <cparsec3/parsec/ParsecFailure1.h>

#include <cparsec3/parsec/ParsecPrim.h>
#include <cparsec3/parsec/ParsecPrim1.h>

#include <cparsec3/parsec/ParsecDeriv.h>

#include <cparsec3/parsec/ParsecChoice.h>

#include <cparsec3/parsec/ParsecChar.h>

#include <cparsec3/parsec/ParsecRepeat.h>

// -----------------------------------------------------------------------
#if 0
#define CPARSEC_STREAM_TYPE String
#define STREAM(input) input
#else
#define CPARSEC_STREAM_TYPE State(String)
#define STREAM(input) trait(State(String)).create(input)
#endif

#if !defined(CPARSEC_STREAM_TYPE)
#define CPARSEC_STREAM_TYPE String
#define STREAM(input) input
#endif

// -----------------------------------------------------------------------
#define PARSER_RETURN_TYPES(S)                                           \
  None, Token(S), Tokens(S), Array(Token(S)), Array(Tokens(S))

#define TRAIT_PARSECRUNNER(S, T) trait(ParsecRunner(S, T))

#define GENERIC_PARSECRUNNER(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECRUNNER, BIND(S, PARSER_RETURN_TYPES(S)))

// -----------------------------------------------------------------------
#define parseTest(p, input)                                              \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p).parseTest(p, input)

#define parse(p, state)                                                  \
  GENERIC_PARSECRUNNER(CPARSEC_STREAM_TYPE, p).runParsec(p, state)

// -----------------------------------------------------------------------
#define DO()                                                             \
  g_bind((_s0_, _cok_, _cerr_, _eok_, _eerr_), *args);                   \
  __auto_type _s_ = _s0_;

#define DO_WITH(...)                                                     \
  g_bind((__VA_ARGS__, _s0_, _cok_, _cerr_, _eok_, _eerr_), *args);      \
  __auto_type _s_ = _s0_;

#define SCAN(...) CAT(SCAN, VARIADIC_SIZE(__VA_ARGS__))(__VA_ARGS__)

#define SCAN1(_p_)                                                       \
  __auto_type TMPID = parse(_p_, _s_);                                   \
  if (!TMPID.result.success) {                                           \
    __auto_type _err_ = (TMPID.consumed ? _cerr_ : _eerr_);              \
    return fn_apply(_err_, TMPID.result.err, TMPID.state);               \
  }                                                                      \
  _s_ = TMPID.state;

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

// -----------------------------------------------------------------------
#define trait_ParsecLibrary(S)                                           \
  trait_ParsecBase(S);                                                   \
                                                                         \
  trait_ParsecRunner(S, None);                                           \
  trait_ParsecRunner(S, Token(S));                                       \
  trait_ParsecRunner(S, Tokens(S));                                      \
  trait_ParsecRunner(S, Array(Token(S)));                                \
  trait_ParsecRunner(S, Array(Tokens(S)));                               \
                                                                         \
  trait_ParsecPrim1(S);                                                  \
  trait_ParsecPrim(S, Token(S));                                         \
  trait_ParsecPrim(S, Tokens(S));                                        \
                                                                         \
  trait_ParsecFailure1(S);                                               \
  trait_ParsecFailure(S, Token(S));                                      \
  trait_ParsecFailure(S, Tokens(S));                                     \
                                                                         \
  trait_ParsecDeriv(S);                                                  \
                                                                         \
  trait_ParsecChoice(S, Token(S));                                       \
  trait_ParsecChoice(S, Tokens(S));                                      \
                                                                         \
  trait_ParsecRepeat(S, Token(S));                                       \
  trait_ParsecRepeat(S, Tokens(S));                                      \
                                                                         \
  trait_ParsecChar(S);                                                   \
                                                                         \
  END_OF_STATEMENTS

#define impl_ParsecLibrary(S)                                            \
  impl_ParsecBase(S);                                                    \
                                                                         \
  impl_ParsecRunner(S, None);                                            \
  impl_ParsecRunner(S, Token(S));                                        \
  impl_ParsecRunner(S, Tokens(S));                                       \
  impl_ParsecRunner(S, Array(Token(S)));                                 \
  impl_ParsecRunner(S, Array(Tokens(S)));                                \
                                                                         \
  impl_ParsecPrim1(S);                                                   \
  impl_ParsecPrim(S, Token(S));                                          \
  impl_ParsecPrim(S, Tokens(S));                                         \
                                                                         \
  impl_ParsecFailure1(S);                                                \
  impl_ParsecFailure(S, Token(S));                                       \
  impl_ParsecFailure(S, Tokens(S));                                      \
                                                                         \
  impl_ParsecDeriv(S);                                                   \
                                                                         \
  impl_ParsecChoice(S, Token(S));                                        \
  impl_ParsecChoice(S, Tokens(S));                                       \
                                                                         \
  impl_ParsecRepeat(S, Token(S));                                        \
  impl_ParsecRepeat(S, Tokens(S));                                       \
                                                                         \
  impl_ParsecChar(S);                                                    \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
trait_ParsecLibrary(CPARSEC_STREAM_TYPE);
impl_ParsecLibrary(CPARSEC_STREAM_TYPE);

// -----------------------------------------------------------------------
#define S CPARSEC_STREAM_TYPE

// -----------------------------------------------------------------------
fn(abcFn, UnParserArgs(S, Array(char))) {
  __auto_type char1 = trait(ParsecChar(S)).char1;
  DO() {
    SCAN(char1('a'), a);
    SCAN(char1('b'), b);
    SCAN(char1('c'), c);
    RETURN(g_array(char, a, b, c));
  }
}

Parsec(S, Array(char)) abc(void) {
  return (Parsec(S, Array(char))){abcFn()};
}

// -----------------------------------------------------------------------
fn(identifierImpl, UnParserArgs(S, String)) {
  __auto_type c = trait(ParsecChar(S));
  __auto_type C = trait(ParsecChoice(S, char));
  __auto_type m = trait(ParsecRepeat(S, char));
  __auto_type identStart = C.either(c.char1('_'), c.letter());
  __auto_type identLetter = m.many(C.either(c.char1('_'), c.alphaNum()));
  __auto_type spaces = m.many(c.whitespace());

  DO() {
    SCAN(identStart, x);
    SCAN(identLetter, xs);
    SCAN(spaces);
    size_t len = 1 + xs.length;
    char* cs = mem_malloc(len + 1);
    cs[0] = x;
    memmove(cs + 1, xs.data, xs.length);
    cs[len] = 0;
    g_free(xs);
    RETURN(cs);
  }
}

Parsec(S, String) identifier(void) {
  __auto_type P = trait(ParsecPrim(S, String));
  Parsec(S, String) p = {identifierImpl()};
  return P.label("identifier", p);
}

// -----------------------------------------------------------------------
typedef_Fn(Token(S), UnParser(S, None));
fn(fail_on_Fn, Token(S), UnParserArgs(S, None)) {
  DO_WITH(c) {
    __auto_type p = trait(ParsecDeriv(S)).anySingleBut(c);
    for (;;) {
      SCAN(p);
    }
    RETURN((None){0});
  }
}

Parsec(S, None) fail_on(Token(S) c) {
  __auto_type f = fail_on_Fn();
  Parsec(S, None) p = {fn_apply(f, c)};
  return p;
}

// -----------------------------------------------------------------------
int main(void) {
  for (int x = 0; x < 256; ++x) {
    String s = trait(Show(char)).show((char)x);
    printf("%-6s ", s);
    mem_free((void*)s);
    if (x % 8 == 7)
      printf("\n");
  }
  printf("\n");

  __auto_type P = trait(ParsecPrim(S, Token(S)));
  __auto_type Q = trait(ParsecPrim(S, Tokens(S)));
  __auto_type D = trait(ParsecDeriv(S));
  {
    __auto_type p = P.parseError((ParseError(S)){0});
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
  }
  {
    __auto_type p = D.single('f');
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
  }
  {
    __auto_type p = D.anySingle();
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
  }
  {
    __auto_type p = D.anySingleBut('f');
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
  }
  {
    __auto_type p = D.chunk("foo");
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
    parseTest(Q.label("foo", p), STREAM("bar"));
  }
  {
    __auto_type C = trait(ParsecChar(S));
    __auto_type p = C.digit();
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
    parseTest(p, STREAM("9"));
  }
  {
    __auto_type C = trait(ParsecChoice(S, Token(S)));
    __auto_type a = D.single('a');
    __auto_type b = D.single('b');
    __auto_type p = C.either(a, b);
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
  }
  {
    __auto_type C = trait(ParsecChoice(S, Token(S)));
    __auto_type a = D.single('a');
    __auto_type b = D.single('b');
    __auto_type p = C.either(P.tryp(a), b);
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
  }
  {
    __auto_type C = trait(ParsecChoice(S, Token(S)));
    __auto_type a = D.single('a');
    __auto_type b = D.single('b');
    __auto_type c = D.single('c');
    __auto_type p =
        C.choice(g_array(Parsec(S, Token(S)), P.tryp(a), P.tryp(b), c));
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
  }
  {
    __auto_type C = trait(ParsecChar(S));
    __auto_type R = trait(ParsecRepeat(S, Token(S)));
    __auto_type p = R.many(C.letter());
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
  }
  {
    __auto_type p = abc();
    parseTest(p, STREAM("abc"));
    parseTest(p, STREAM("bcd"));
    parseTest(p, STREAM("aBc"));
  }
  {
    __auto_type p = identifier();
    parseTest(p, STREAM(""));
    parseTest(p, STREAM("foo"));
    parseTest(p, STREAM("bar"));
    parseTest(p, STREAM("9bar"));
    parseTest(p, STREAM("bar9"));
  }
  {
    __auto_type p = fail_on('X');
    parseTest(p, STREAM("0123456789abcdef\n"
                        "0123456789abcdef\n"
                        "0123456789aXcdef\n"));
    // -> shall be an error at line 3, column 12
    parseTest(p, STREAM("0123456789abcdef\n"
                        "0123456789abcdef\n"
                        "\t89aXcdef\n"));
    // -> shall be an error at line 3, column 12
    parseTest(p, STREAM("0123456789abcdef\n"
                        "0123456789abcdef\n"
                        "01\t89aXcdef\n"));
    // -> shall be an error at line 3, column 12
    parseTest(p, STREAM("0123456789abcdef\n"
                        "0123456789abcdef\n"
                        "0123456789abcdef\n"));
    // -> shall be an error at line 4, column 1 : unexpected end of input
  }
}
