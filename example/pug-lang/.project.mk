# -*- coding: utf-8-unix -*-
TARGET = bin/pug

CFLAGS   += -D_POSIX_C_SOURCE
CFLAGS   += -std=c11 -I include -I ../../include
CFLAGS   += -ftrack-macro-expansion=0
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec3

TESTFLAGS = --self-test

depend:
	$(MAKE) lib -C ../..
