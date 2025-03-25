CC=gcc

CFLAGS=-std=c11 -pedantic -pedantic-errors -Wno-parentheses

FREETYPE2_INCLUDES=-I/usr/include/freetype2
FREETYPE2_INCLUDES+=-I/usr/include/libpng16

FREETYPE2_LIBS=-lfreetype

CFLAGS+=$(FREETYPE2_INCLUDES)

LDLIBS=$(FREETYPE2_LIBS)

all: ft

clean:
	-rm -f ft ft.o
