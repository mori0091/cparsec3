# -*- coding: utf-8-unix -*-
TARGET = bin/vmtest

CFLAGS   += -D_POSIX_C_SOURCE
CFLAGS   += -std=c11
CFLAGS   += -ftrack-macro-expansion=0
CFLAGS   += -O2
CFLAGS   += -I include

CFLAGS   += -I ../include
LDFLAGS  += -L ../lib
LDLIBS   += -lvm

CFLAGS   += -I ../../../include
LDFLAGS  += -L ../../../lib
LDLIBS   += -lcparsec3

CFLAGS   += -I ../../../test/testit/include

# TESTFLAGS =

depend:
	$(MAKE) lib -C ..
