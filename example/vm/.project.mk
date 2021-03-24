# -*- coding: utf-8-unix -*-
TARGET = bin/vm

CFLAGS   += -D_POSIX_C_SOURCE
CFLAGS   += -std=c11 -I include -I ../../include
CFLAGS   += -I ../../test/testit/include
CFLAGS   += -ftrack-macro-expansion=0
CFLAGS   += -O2
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec3

# TESTFLAGS =

depend:
	$(MAKE) lib -C ../..
