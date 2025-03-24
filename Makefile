CC=clang

CFLAGS=-std=c17 -pedantic -pedantic-errors -Wno-parentheses

FREETYPE2_INCLUDES = -I/usr/include/freetype2 -I/usr/include/libpng16
FREETYPE2_LIBS = -lfreetype

CFLAGS+=$(FREETYPE2_INCLUDES) -I/usr/include/libpng16 $(FREETYPE2_LIBS)

all: ft

clean:
	-rm -f ft
