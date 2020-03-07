/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/base.h"

typedef struct SourcePos SourcePos;
struct SourcePos {
  String name;
  int line;
  int column;
};
