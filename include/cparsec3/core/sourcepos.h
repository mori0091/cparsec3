/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

typedef struct {
  String name;
  Offset line;
  Offset column;
} SourcePos;

static inline SourcePos initialSourcePos(String name) {
  return (SourcePos){.name = name, .line = 1, .column = 1};
}
