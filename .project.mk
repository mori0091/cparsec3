# -*- coding:utf-8-unix -*-
# TARGET    = bin/cparsec3
LIBTARGET = lib/libcparsec3.a

CFLAGS   += -D_POSIX_C_SOURCE=200809L
CFLAGS   += -std=c11 -I include
LDFLAGS  +=
LDLIBS   +=
