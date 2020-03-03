/* -*- coding: utf-8-unix -*- */
#include <stdint.h>
#include <ctype.h>

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
