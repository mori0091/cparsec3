/* -*- coding: utf-8-unix -*- */
// #include <cparsec3/base/base.h>
#include <cparsec3/base/base_generics.h>

#include <cparsec3/parsec/ParsecBase.h>
#include <cparsec3/parsec/ParsecRunner.h>
#include <cparsec3/parsec/ParsecPrim.h>
#include <cparsec3/parsec/ParsecFailure.h>
#include <cparsec3/parsec/ParsecDeriv.h>

// -----------------------------------------------------------------------
#include "cparsec3/stream/stream_string.h"

#define S String

trait_ParsecBase(S);
trait_ParsecPrim(S, Token(S));
trait_ParsecFailure(S, Token(S));
trait_ParsecDeriv(S);

// trait_ParsecRunner(S, None);
trait_ParsecRunner(S, Token(S));
// trait_ParsecRunner(S, Tokens(S));

impl_ParsecBase(S);
impl_ParsecPrim(S, Token(S));
impl_ParsecFailure(S, Token(S));
impl_ParsecDeriv(S);

// impl_ParsecRunner(S, None);
impl_ParsecRunner(S, Token(S));
// impl_ParsecRunner(S, Tokens(S));

int main(void) {
  __auto_type R = trait(ParsecRunner(S, Token(S)));
  __auto_type P = trait(ParsecPrim(S, Token(S)));
  __auto_type D = trait(ParsecDeriv(S));

  __auto_type p1 = P.parseError((ParseError(S)){0});
  R.parseTest(p1, "", "");
  R.parseTest(p1, "", "foo");
  R.parseTest(p1, "", "bar");

  __auto_type p2 = D.single('f');
  R.parseTest(p2, "", "");
  R.parseTest(p2, "", "foo");
  R.parseTest(p2, "", "bar");

  __auto_type p3 = D.anySingle();
  R.parseTest(p3, "", "");
  R.parseTest(p3, "", "foo");
  R.parseTest(p3, "", "bar");

  __auto_type p4 = D.anySingleBut('f');
  R.parseTest(p4, "", "");
  R.parseTest(p4, "", "foo");
  R.parseTest(p4, "", "bar");
}
