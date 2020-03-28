/* -*- coding: utf-8-unix -*- */

#define CPARSEC_CONFIG_USER_TYPES int
// #define CPARSEC_CONFIG_DATA_SOURCE String
// #define CPARSEC_CONFIG_POSITIONER Text  /* line/column */
// #define CPARSEC_CONFIG_POSITIONER Index /* index */
// #define CPARSEC_CONFIG_POSITIONER       /* memory address or else */
#define CPARSEC_CONFIG_IMPLEMENT /* generate parsec code if defined */

#include <cparsec3/easy_parsec/parser/char.h>
#include <cparsec3/easy_parsec/parser/choice.h>
#include <cparsec3/easy_parsec/parser/combinator.h>
#include <cparsec3/easy_parsec/parser/failure.h>
#include <cparsec3/easy_parsec/parser/repeat.h>
#include <cparsec3/easy_parsec/parser/token.h>

// -----------------------------------------------------------------------
#define S CPARSEC_STREAM_TYPE

#include <limits.h>

// -----------------------------------------------------------------------
// PARSER(int) number0(void);
parsec(number0, int) {
  DO() {
    SCAN(some(digit()), xs);
    ArrayT(char) A = trait(Array(char));
    int y = 0;
    for (char* p = A.begin(xs); p != A.end(xs); p++) {
      int x = *p - '0';
      if (y > INT_MAX / 10 || (y == INT_MAX / 10 && x > INT_MAX % 10)) {
        FAIL("too large number");
      }
      y = 10 * y + x;
    }
    RETURN(y);
  }
}

PARSER(int) number(void) {
  return label("a number [0..INT_MAX]", tryp(number0()));
}

// -----------------------------------------------------------------------
// PARSER(Array(char)) abc(void);
parsec(abc, Array(char)) {
  DO() {
    SCAN(char1('a'), a);
    SCAN(char1('b'), b);
    SCAN(char1('c'), c);
    RETURN(g_array(char, a, b, c));
  }
}

// -----------------------------------------------------------------------
// PARSER(String) identifier0(void);
parsec(identifier0, String) {
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

PARSER(String) identifier(void) {
  return label("identifier", identifier0());
}

// -----------------------------------------------------------------------
// PARSER(None) fail_on(Token(S) c);
parsec(fail_on, Token(S), None) {
  DO() WITH(c) {
    __auto_type p = anySingleBut(c);
    for (;;) {
      SCAN(p);
    }
    RETURN((None){0});
  }
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
    __auto_type p = parseError(Token(S), (ParseError(S)){0});
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
  {
    __auto_type p = number();
    parseTest(p, "");
    parseTest(p, "123");
    parseTest(p, "123a");
    parseTest(p, "a123");
    parseTest(p, "99999999999999999999");
  }
  {
    parseTest(count(-1, digit()), "");    /* -> [] */
    parseTest(count(0, digit()), "");     /* -> [] */
    parseTest(count(1, digit()), "");     /* -> unexpected end of input */
    parseTest(count(2, digit()), "123a"); /* -> ['1', '2'] */
    parseTest(count(3, digit()), "123a"); /* -> ['1', '2', '3'] */
    parseTest(count(4, digit()), "123a"); /* -> unexpected 'a' */
  }
  {
    parseTest(count_min_max(1, 0, digit()), ""); /* -> [] */
    parseTest(count_min_max(0, 0, digit()), ""); /* -> [] */
    parseTest(count_min_max(0, 1, digit()), ""); /* -> [] */
    parseTest(count_min_max(1, 1, digit()), "");
    /* -> unexpected end of input */
    parseTest(count_min_max(1, 2, digit()), "123a");
    /* -> ['1', '2'] */
    parseTest(count_min_max(1, 3, digit()), "123a");
    /* -> ['1', '2', '3'] */
    parseTest(count_min_max(1, 4, digit()), "123a");
    /* -> ['1', '2', '3'] */
    parseTest(count_min_max(4, 4, digit()), "123a");
    /* -> unexpected 'a' */
  }
}
