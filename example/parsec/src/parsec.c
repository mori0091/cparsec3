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

// -----------------------------------------------------------------------
#include "cparsec3/stream/stream_string.h"

#define PARSER_RETURN_TYPES(S) Token(S), Tokens(S), None

#define TRAIT_PARSECRUNNER(S, T) trait(ParsecRunner(S, T))
#define GENERIC_PARSECRUNNER(S, p)                                       \
  GENERIC(p, Parsec, TRAIT_PARSECRUNNER, BIND(S, PARSER_RETURN_TYPES(S)))

#define trait_ParsecLibrary(S)                                           \
  trait_ParsecBase(S);                                                   \
                                                                         \
  trait_ParsecRunner(S, Token(S));                                       \
  trait_ParsecRunner(S, Tokens(S));                                      \
  trait_ParsecRunner(S, None);                                           \
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
  END_OF_STATEMENTS

#define impl_ParsecLibrary(S)                                            \
  impl_ParsecBase(S);                                                    \
                                                                         \
  impl_ParsecRunner(S, Token(S));                                        \
  impl_ParsecRunner(S, Tokens(S));                                       \
  impl_ParsecRunner(S, None);                                            \
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
  END_OF_STATEMENTS

trait_ParsecLibrary(String);
impl_ParsecLibrary(String);

// -----------------------------------------------------------------------
#define S String

#define g_parseTest(p, input)                                            \
  GENERIC_PARSECRUNNER(String, p).parseTest(p, input)

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
  __auto_type D = trait(ParsecDeriv(S));
  {
    __auto_type p = P.parseError((ParseError(S)){0});
    g_parseTest(p, "");
    g_parseTest(p, "foo");
    g_parseTest(p, "bar");
  }
  {
    __auto_type p = D.single('f');
    g_parseTest(p, "");
    g_parseTest(p, "foo");
    g_parseTest(p, "bar");
  }
  {
    __auto_type p = D.anySingle();
    g_parseTest(p, "");
    g_parseTest(p, "foo");
    g_parseTest(p, "bar");
  }
  {
    __auto_type p = D.anySingleBut('f');
    g_parseTest(p, "");
    g_parseTest(p, "foo");
    g_parseTest(p, "bar");
  }
  {
    __auto_type p = D.chunk("foo");
    g_parseTest(p, "");
    g_parseTest(p, "foo");
    g_parseTest(p, "bar");
  }
}
