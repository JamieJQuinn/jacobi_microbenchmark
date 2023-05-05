CFLAGS=-Wall -Wextra -lm
LFLAGS=-lm

EXES=reference.x

%.x: %.c
	gcc ${CFLAGS} $< -o $@

all: ${EXES}

clean:
	rm ${EXES}

debug: CFLAGS+=-g
debug: all
