# -*- coding: utf-8-unix -*-
# TARGET = bin/vm
LIBTARGET = lib/libvm.a

CFLAGS   += -D_POSIX_C_SOURCE
CFLAGS   += -std=c11
CFLAGS   += -ftrack-macro-expansion=0
CFLAGS   += -O2

CFLAGS   += -I include

CFLAGS   += -I ../../include
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec3

# TESTFLAGS =

depend:
	$(MAKE) lib -s -C ../..

test::
	$(MAKE) test -s -C test

clean::
	@$(MAKE) clean -s -C test

cov::
	$(MAKE) cov -s -C test

gcov::
	$(MAKE) gcov -s -C test
