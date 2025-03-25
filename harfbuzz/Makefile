CC=gcc

## No --pedantic: Some harfbuzz headers violate it.
CFLAGS=-std=c11 -Wall -Wno-parentheses

# HARFBUZZ_INCLUDES=-I/usr/include/harfbuzz

HARFBUZZ_INCLUDES=-I/work/github/harfbuzz/harfbuzz/src

HARFBUZZ_LIBS=-L/work/github/harfbuzz/harfbuzz/build/src -lharfbuzz

CFLAGS+=$(HARFBUZZ_INCLUDES)

LDLIBS=$(HARFBUZZ_LIBS)

all: hb

clean:
	-rm -f hb hb.o
