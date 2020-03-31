/* -*- coding: utf-8-unix -*- */
#pragma once

#include "ParsecToken1.h"

// -----------------------------------------------------------------------
#define ParsecChar(...) TYPE_NAME(ParsecChar, __VA_ARGS__)

// -----------------------------------------------------------------------
#define trait_ParsecChar(S)                                              \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct ParsecChar(S) ParsecChar(S);                            \
  struct ParsecChar(S) {                                                 \
    Parsec(S, char) (*pNewline)(void);                                   \
    Parsec(S, String) (*pCrlf)(void);                                    \
    Parsec(S, String) (*pEol)(void);                                     \
    Parsec(S, char) (*pTab)(void);                                       \
    Parsec(S, None) (*pSpace)(void);                                     \
    Parsec(S, None) (*pSpace1)(void);                                    \
                                                                         \
    Parsec(S, char) (*pControl)(void);                                   \
    Parsec(S, char) (*pWhitespace)(void);                                \
    Parsec(S, char) (*pUpper)(void);                                     \
    Parsec(S, char) (*pLower)(void);                                     \
    Parsec(S, char) (*pLetter)(void);                                    \
    Parsec(S, char) (*pAlphaNum)(void);                                  \
    Parsec(S, char) (*pPrintable)(void);                                 \
    Parsec(S, char) (*pDigit)(void);                                     \
    Parsec(S, char) (*pBinDigit)(void);                                  \
    Parsec(S, char) (*pOctDigit)(void);                                  \
    Parsec(S, char) (*pHexDigit)(void);                                  \
                                                                         \
    Parsec(S, char) (*pChar1)(char c);                                   \
    Parsec(S, String) (*pString1)(String s);                             \
  };                                                                     \
                                                                         \
  ParsecChar(S) Trait(ParsecChar(S));                                    \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_ParsecChar(S)                                               \
  C_API_BEGIN                                                            \
                                                                         \
  impl_space(S);                                                         \
                                                                         \
  static Parsec(S, char) FUNC_NAME(newline, S)(void) {                   \
    return trait(ParsecChar(S)).pChar1('\n');                            \
  }                                                                      \
  static Parsec(S, String) FUNC_NAME(crlf, S)(void) {                    \
    return trait(ParsecChar(S)).pString1("\r\n");                        \
  }                                                                      \
  static Parsec(S, String) FUNC_NAME(eol, S)(void) {                     \
    ParsecChar(S) c = trait(ParsecChar(S));                              \
    ParsecChoice(S, String) C = trait(ParsecChoice(S, String));          \
    return C.pEither(c.pString1("\n"), c.pCrlf());                       \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(tab, S)(void) {                       \
    return trait(ParsecChar(S)).pChar1('\t');                            \
  }                                                                      \
                                                                         \
  /* Fn(char, bool) make_pred(bool (*f)(char)) */                        \
  fp(make_pred, char, bool);                                             \
                                                                         \
  static Parsec(S, char) char_category(String l, Fp(char, bool) p) {     \
    ParsecCombinator(S, char) C = trait(ParsecCombinator(S, char));      \
    ParsecToken1(S) D = trait(ParsecToken1(S));                          \
    return C.pLabel(l, D.pSatisfy(make_pred(p)));                        \
  }                                                                      \
                                                                         \
  static Parsec(S, char) FUNC_NAME(control, S)(void) {                   \
    return char_category("control character", is_control);               \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(whitespace, S)(void) {                \
    return char_category("white space", is_space);                       \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(upper, S)(void) {                     \
    return char_category("uppercase letter", is_upper);                  \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(lower, S)(void) {                     \
    return char_category("lowercase letter", is_lower);                  \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(letter, S)(void) {                    \
    return char_category("letter", is_letter);                           \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(alphaNum, S)(void) {                  \
    return char_category("alphanumeric character", is_alnum);            \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(printable, S)(void) {                 \
    return char_category("printable character", is_print);               \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(digit, S)(void) {                     \
    return char_category("digit", is_digit);                             \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(binDigit, S)(void) {                  \
    return char_category("binary digit", is_binDigit);                   \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(octDigit, S)(void) {                  \
    return char_category("octal digit", is_octDigit);                    \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(hexDigit, S)(void) {                  \
    return char_category("hexadecimal digit", is_hexDigit);              \
  }                                                                      \
                                                                         \
  ParsecChar(S) Trait(ParsecChar(S)) {                                   \
    return (ParsecChar(S)){                                              \
        .pNewline = FUNC_NAME(newline, S),                               \
        .pCrlf = FUNC_NAME(crlf, S),                                     \
        .pEol = FUNC_NAME(eol, S),                                       \
        .pTab = FUNC_NAME(tab, S),                                       \
        .pSpace = FUNC_NAME(space, S),                                   \
        .pSpace1 = 0,                                                    \
                                                                         \
        .pControl = FUNC_NAME(control, S),                               \
        .pWhitespace = FUNC_NAME(whitespace, S),                         \
        .pUpper = FUNC_NAME(upper, S),                                   \
        .pLower = FUNC_NAME(lower, S),                                   \
        .pLetter = FUNC_NAME(letter, S),                                 \
        .pAlphaNum = FUNC_NAME(alphaNum, S),                             \
        .pPrintable = FUNC_NAME(printable, S),                           \
        .pDigit = FUNC_NAME(digit, S),                                   \
        .pBinDigit = FUNC_NAME(binDigit, S),                             \
        .pOctDigit = FUNC_NAME(octDigit, S),                             \
        .pHexDigit = FUNC_NAME(hexDigit, S),                             \
                                                                         \
        .pChar1 = trait(ParsecToken1(S)).pSingle,                        \
        .pString1 = trait(ParsecToken1(S)).pChunk,                       \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_space(S)                                                    \
  fn(FUNC_NAME(spaceImpl, S), UnParserArgs(S, None)) {                   \
    g_bind((s, cok, , eok), *args);                                      \
    Stream(S) SS = trait(Stream(S));                                     \
    bool consumed = false;                                               \
    for (;;) {                                                           \
      Maybe(Tuple(Token(S), S)) m = SS.take1(s);                         \
      if (m.none || !is_space(m.value.e1)) {                             \
        break;                                                           \
      }                                                                  \
      s = m.value.e2;                                                    \
      consumed = true;                                                   \
    }                                                                    \
    return fn_apply((consumed ? cok : eok), (None){0}, s,                \
                    (Hints(S)){0});                                      \
  }                                                                      \
                                                                         \
  static Parsec(S, None) FUNC_NAME(space, S)(void) {                     \
    return (Parsec(S, None)){FUNC_NAME(spaceImpl, S)()};                 \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

static inline bool is_control(char c) {
  return !!iscntrl((int)(uint8_t)c);
}
static inline bool is_space(char c) {
  return !!isspace((int)(uint8_t)c);
}
static inline bool is_upper(char c) {
  return !!isupper((int)(uint8_t)c);
}
static inline bool is_lower(char c) {
  return !!islower((int)(uint8_t)c);
}
static inline bool is_letter(char c) {
  return !!isalpha((int)(uint8_t)c);
}
static inline bool is_alnum(char c) {
  return !!isalnum((int)(uint8_t)c);
}
static inline bool is_print(char c) {
  return !!isprint((int)(uint8_t)c);
}
static inline bool is_digit(char c) {
  return !!isdigit((int)(uint8_t)c);
}
static inline bool is_binDigit(char c) {
  return (c == '0' || c == '1');
}
static inline bool is_octDigit(char c) {
  return ((char)'0' <= c && c <= (char)'7');
}
static inline bool is_hexDigit(char c) {
  return !!isxdigit((int)(uint8_t)c);
}
