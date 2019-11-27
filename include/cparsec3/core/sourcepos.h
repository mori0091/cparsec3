/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

typedef struct SourcePos SourcePos;
struct SourcePos {
  String name;
  Offset line;
  Offset column;
};

static inline SourcePos initialSourcePos(String name) {
  return (SourcePos){.name = name, .line = 1, .column = 1};
}
