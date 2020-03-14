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
    Parsec(S, char) (*newline)(void);                                    \
    Parsec(S, String) (*crlf)(void);                                     \
    Parsec(S, String) (*eol)(void);                                      \
    Parsec(S, char) (*tab)(void);                                        \
    Parsec(S, None) (*space)(void);                                      \
    Parsec(S, None) (*space1)(void);                                     \
                                                                         \
    Parsec(S, char) (*control)(void);                                    \
    Parsec(S, char) (*whitespace)(void);                                 \
    Parsec(S, char) (*upper)(void);                                      \
    Parsec(S, char) (*lower)(void);                                      \
    Parsec(S, char) (*letter)(void);                                     \
    Parsec(S, char) (*alphaNum)(void);                                   \
    Parsec(S, char) (*printable)(void);                                  \
    Parsec(S, char) (*digit)(void);                                      \
    Parsec(S, char) (*binDigit)(void);                                   \
    Parsec(S, char) (*octDigit)(void);                                   \
    Parsec(S, char) (*hexDigit)(void);                                   \
                                                                         \
    Parsec(S, char) (*char1)(char c);                                    \
    Parsec(S, String) (*string1)(String s);                              \
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
  static Parsec(S, char) FUNC_NAME(newline, S)(void) {                   \
    return trait(ParsecChar(S)).char1('\n');                             \
  }                                                                      \
  static Parsec(S, String) FUNC_NAME(crlf, S)(void) {                    \
    return trait(ParsecChar(S)).string1("\r\n");                         \
  }                                                                      \
  static Parsec(S, String) FUNC_NAME(eol, S)(void) {                     \
    ParsecChar(S) c = trait(ParsecChar(S));                              \
    ParsecChoice(S, String) C = trait(ParsecChoice(S, String));          \
    return C.either(c.string1("\n"), c.crlf());                          \
  }                                                                      \
  static Parsec(S, char) FUNC_NAME(tab, S)(void) {                       \
    return trait(ParsecChar(S)).char1('\t');                             \
  }                                                                      \
                                                                         \
  /* Fn(char, bool) make_pred(bool (*f)(char)) */                        \
  fp(make_pred, char, bool);                                             \
                                                                         \
  static Parsec(S, char) char_category(String l, Fp(char, bool) p) {     \
    ParsecCombinator(S, char) C = trait(ParsecCombinator(S, char));      \
    ParsecToken1(S) D = trait(ParsecToken1(S));                          \
    return C.label(l, D.satisfy(make_pred(p)));                          \
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
        .newline = FUNC_NAME(newline, S),                                \
        .crlf = FUNC_NAME(crlf, S),                                      \
        .eol = FUNC_NAME(eol, S),                                        \
        .tab = FUNC_NAME(tab, S),                                        \
        .space = 0,                                                      \
        .space1 = 0,                                                     \
                                                                         \
        .control = FUNC_NAME(control, S),                                \
        .whitespace = FUNC_NAME(whitespace, S),                          \
        .upper = FUNC_NAME(upper, S),                                    \
        .lower = FUNC_NAME(lower, S),                                    \
        .letter = FUNC_NAME(letter, S),                                  \
        .alphaNum = FUNC_NAME(alphaNum, S),                              \
        .printable = FUNC_NAME(printable, S),                            \
        .digit = FUNC_NAME(digit, S),                                    \
        .binDigit = FUNC_NAME(binDigit, S),                              \
        .octDigit = FUNC_NAME(octDigit, S),                              \
        .hexDigit = FUNC_NAME(hexDigit, S),                              \
                                                                         \
        .char1 = trait(ParsecToken1(S)).single,                          \
        .string1 = trait(ParsecToken1(S)).chunk,                         \
    };                                                                   \
  }                                                                      \
                                                                         \
  C_API_END                                                              \
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
  return ((char)'0' <= c || c <= (char)'7');
}
static inline bool is_hexDigit(char c) {
  return !!isxdigit((int)(uint8_t)c);
}
