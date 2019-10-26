# -*- coding: utf-8-unix -*-
TARGET = AllTests

CFLAGS   += -D_POSIX_C_SOURCE
CFLAGS   += -std=c11 -I include -I ../../include
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec3

$(TARGET): ../../lib/libcparsec3.a

../../lib/libcparsec3.a: depend

depend:
	$(MAKE) lib -C ../..
