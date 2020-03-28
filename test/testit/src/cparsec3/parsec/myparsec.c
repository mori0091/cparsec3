/* -*- coding: utf-8-unix -*- */

/* generate parsec code if defined */
#define CPARSEC_CONFIG_IMPLEMENT
#include "myparsec.h"

parsec(abc, String) {
  DO() {
    SCAN(char1('a'));
    SCAN(char1('b'));
    SCAN(char1('c'));
    RETURN("abc");
  }
}
