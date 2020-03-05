/* -*- coding: utf-8-unix -*- */
// #include <cparsec3/base/base.h>
#include <cparsec3/base/base_generics.h>

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
#include "cparsec3/stream/stream_string.h"

#define PARSER_RETURN_TYPES(S)                                           \
  None, Token(S), Tokens(S), Array(Token(S)), Array(Tokens(S))

#define TRAIT_PARSECRUNNER(S, T) trait(ParsecRunner(S, T))
#define GENERIC_PARSECRUNNER(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECRUNNER, BIND(S, PARSER_RETURN_TYPES(S)))

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

trait_ParsecLibrary(String);
impl_ParsecLibrary(String);

// -----------------------------------------------------------------------
#define S String

#define parseTest(p, input)                                              \
  GENERIC_PARSECRUNNER(String, p).parseTest(p, input)

#define parse(p, state)                                                  \
  GENERIC_PARSECRUNNER(String, p).runParsec(p, state)

// -----------------------------------------------------------------------
#define DO()                                                             \
  g_bind((_s0_, _cok_, _cerr_, _eok_, _eerr_), *args);                   \
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
    __auto_type _ok_ = (_s0_.offset < _s_.offset ? _cok_ : _eok_);       \
    return fn_apply(_ok_, _x_, _s_, (Hints(Token(S))){0});               \
  } while (0)

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
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = D.single('f');
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = D.anySingle();
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = D.anySingleBut('f');
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = D.chunk("foo");
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
    parseTest(Q.label("foo", p), "bar");
  }
  {
    __auto_type C = trait(ParsecChar(S));
    __auto_type p = C.digit();
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
    parseTest(p, "9");
  }
  {
    __auto_type C = trait(ParsecChoice(S, Token(S)));
    __auto_type a = D.single('a');
    __auto_type b = D.single('b');
    __auto_type p = C.either(a, b);
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type C = trait(ParsecChoice(S, Token(S)));
    __auto_type a = D.single('a');
    __auto_type b = D.single('b');
    __auto_type p = C.either(P.tryp(a), b);
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type C = trait(ParsecChoice(S, Token(S)));
    __auto_type a = D.single('a');
    __auto_type b = D.single('b');
    __auto_type c = D.single('c');
    __auto_type p =
        C.choice(g_array(Parsec(S, Token(S)), P.tryp(a), P.tryp(b), c));
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type C = trait(ParsecChar(S));
    __auto_type R = trait(ParsecRepeat(S, Token(S)));
    __auto_type p = R.many(C.letter());
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = identifier();
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
    parseTest(p, "9bar");
    parseTest(p, "bar9");
  }
}
