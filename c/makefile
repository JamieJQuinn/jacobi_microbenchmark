PRECISION=double
COMPILER=gcc
CFLAGS=-Wall -Wextra -DPRECISION=${PRECISION}
LFLAGS=-lm
OFLAGS=-O3 -march=native
reference_name=$(basename ${shell ./ls_latest.sh})

EXES=$(subst .c,.x,$(shell ls v*.c))

.PHONY: build run all vary_flags run clean debug

build: ${EXES}

run: build
	run_all.sh

all: run

vary_flags: ${reference_name}.x ${reference_name}_O1.x ${reference_name}_O2.x ${reference_name}_O3.x ${reference_name}_O3_native.x ${reference_name}_Ofast_native.x

clean:
	rm *.x *.csv

debug: CFLAGS+=-g
debug: all

%.x: %.c
	${COMPILER} ${CFLAGS} ${OFLAGS} $< -o $@ ${LFLAGS}

${reference_name}_O1.x: ${reference_name}.c
	${COMPILER} ${CFLAGS} -O1 $< -o $@ ${LFLAGS}

${reference_name}_O2.x: ${reference_name}.c
	${COMPILER} ${CFLAGS} -O2 $< -o $@ ${LFLAGS}

${reference_name}_O3.x: ${reference_name}.c
	${COMPILER} ${CFLAGS} -O3 $< -o $@ ${LFLAGS}

${reference_name}_O3_native.x: ${reference_name}.c
	${COMPILER} ${CFLAGS} -O3 -march=native $< -o $@ ${LFLAGS}

${reference_name}_Ofast_native.x: ${reference_name}.c
	${COMPILER} ${CFLAGS} -Ofast -march=native $< -o $@ ${LFLAGS}
