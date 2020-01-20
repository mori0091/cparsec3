/* -*- coding: utf-8-unix -*- */
// #include <cparsec3/base/base.h>
#include <cparsec3/base/base_generics.h>

#include <cparsec3/parsec/ParsecBase.h>
#include <cparsec3/parsec/ParsecDeriv.h>
#include <cparsec3/parsec/ParsecFailure.h>
#include <cparsec3/parsec/ParsecPrim.h>
#include <cparsec3/parsec/ParsecRunner.h>

// -----------------------------------------------------------------------
#include "cparsec3/stream/stream_string.h"

#define S String

trait_ParsecBase(S);
trait_ParsecPrim(S, Token(S));
trait_ParsecFailure(S, Token(S));
trait_ParsecDeriv(S);

// trait_ParsecRunner(S, None);
trait_ParsecRunner(S, Token(S));
trait_ParsecRunner(S, Tokens(S));

impl_ParsecBase(S);
impl_ParsecPrim(S, Token(S));
impl_ParsecFailure(S, Token(S));
impl_ParsecDeriv(S);

// impl_ParsecRunner(S, None);
impl_ParsecRunner(S, Token(S));
impl_ParsecRunner(S, Tokens(S));

int main(void) {
  for (int x = 0; x < 256; ++x) {
    String s = trait(Show(char)).show((char)x);
    printf("%-6s ", s);
    mem_free((void*)s);
    if (x % 8 == 7)
      printf("\n");
  }
  printf("\n");

  __auto_type R = trait(ParsecRunner(S, Token(S)));
  __auto_type P = trait(ParsecPrim(S, Token(S)));
  __auto_type D = trait(ParsecDeriv(S));
  {
    __auto_type p = P.parseError((ParseError(S)){0});
    R.parseTest(p, "");
    R.parseTest(p, "foo");
    R.parseTest(p, "bar");
  }
  {
    __auto_type p = D.single('f');
    R.parseTest(p, "");
    R.parseTest(p, "foo");
    R.parseTest(p, "bar");
  }
  {
    __auto_type p = D.anySingle();
    R.parseTest(p, "");
    R.parseTest(p, "foo");
    R.parseTest(p, "bar");
  }
  {
    __auto_type p = D.anySingleBut('f');
    R.parseTest(p, "");
    R.parseTest(p, "foo");
    R.parseTest(p, "bar");
  }
  {
    __auto_type R = trait(ParsecRunner(S, Tokens(S)));
    __auto_type p = D.chunk("foo");
    R.parseTest(p, "");
    R.parseTest(p, "foo");
    R.parseTest(p, "bar");
  }
}
