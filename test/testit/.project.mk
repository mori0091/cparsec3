# -*- coding: utf-8-unix -*-
TARGET = bin/AllTests

CFLAGS   += -D_POSIX_C_SOURCE
CFLAGS   += -std=c11 -I include -I ../../include
# CFLAGS   += -DCPARSEC_ENABLE_NESTED_CONTAINER
CFLAGS   += -ftrack-macro-expansion=0
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec3

depend:
	$(MAKE) lib -C ../..
