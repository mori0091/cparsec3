/* -*- coding: utf-8-unix -*- */
#include <cparsec3/easy_parsec/setup.h>
#include <cparsec3/easy_parsec/types.h>

#include <cparsec3/easy_parsec/runner.h>
#include <cparsec3/easy_parsec/parser/char.h>
#include <cparsec3/easy_parsec/parser/choice.h>
#include <cparsec3/easy_parsec/parser/combinator.h>
#include <cparsec3/easy_parsec/parser/repeat.h>
#include <cparsec3/easy_parsec/parser/token.h>

// -----------------------------------------------------------------------
#define S CPARSEC_STREAM_TYPE

// -----------------------------------------------------------------------
fn(abcFn, UnParserArgs(S, Array(char))) {
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
  __auto_type identStart = either(char1('_'), letter());
  __auto_type identLetter = many(either(char1('_'), alphaNum()));
  __auto_type spaces = many(whitespace());

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
  Parsec(S, String) p = {identifierImpl()};
  return label("identifier", p);
}

// -----------------------------------------------------------------------
typedef_Fn(Token(S), UnParser(S, None));
fn(fail_on_Fn, Token(S), UnParserArgs(S, None)) {
  DO_WITH(c) {
    __auto_type p = anySingleBut(c);
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

  {
    __auto_type F = trait(ParsecFailure(S, Token(S)));
    __auto_type p = F.parseError((ParseError(S)){0});
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = single('f');
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = anySingle();
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = anySingleBut('f');
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = chunk("foo");
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
    parseTest(label("foo", p), "bar");
  }
  {
    __auto_type p = digit();
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
    parseTest(p, "9");
  }
  {
    __auto_type a = single('a');
    __auto_type b = single('b');
    __auto_type p = either(a, b);
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type a = single('a');
    __auto_type b = single('b');
    __auto_type p = either(tryp(a), b);
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type a = single('a');
    __auto_type b = single('b');
    __auto_type c = single('c');
    __auto_type p = choice(tryp(a), tryp(b), c);
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type foo = chunk("foo");
    __auto_type bar = chunk("bar");
    __auto_type baz = chunk("baz");
    __auto_type p = choice(foo, bar, baz);
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
    parseTest(p, "boo");
  }
  {
    __auto_type foo = string1("foo");
    __auto_type bar = string1("bar");
    __auto_type baz = string1("baz");
    __auto_type p = choice(foo, bar, baz);
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
    parseTest(p, "boo");
  }
  {
    __auto_type p = many(letter());
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
  }
  {
    __auto_type p = abc();
    parseTest(p, "abc");
    parseTest(p, "bcd");
    parseTest(p, "aBc");
  }
  {
    __auto_type p = identifier();
    parseTest(p, "");
    parseTest(p, "foo");
    parseTest(p, "bar");
    parseTest(p, "9bar");
    parseTest(p, "bar9");
  }
  {
    __auto_type p = fail_on('X');
    parseTest(p, "0123456789abcdef\n"
                 "0123456789abcdef\n"
                 "0123456789aXcdef\n");
    // -> shall be an error at line 3, column 12
    parseTest(p, "0123456789abcdef\n"
                 "0123456789abcdef\n"
                 "\t89aXcdef\n");
    // -> shall be an error at line 3, column 12
    parseTest(p, "0123456789abcdef\n"
                 "0123456789abcdef\n"
                 "01\t89aXcdef\n");
    // -> shall be an error at line 3, column 12
    parseTest(p, "0123456789abcdef\n"
                 "0123456789abcdef\n"
                 "0123456789abcdef\n");
    // -> shall be an error at line 4, column 1 : unexpected end of input
  }
}
