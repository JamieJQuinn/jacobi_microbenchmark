RUN_REPEATS=10
PRECISION=double
COMPILER=g++
CFLAGS=-Wall -Wextra -DPRECISION=${PRECISION} -fno-exceptions -fno-rtti
LFLAGS=-lm
OFLAGS=-O3 -march=native
reference_name:=$(basename ${shell ./ls_latest.sh})

EXES=$(subst .cpp,.x,$(shell ls v*.cpp))
CSVS=$(subst .cpp,.csv,$(shell ls v*.cpp))

.PHONY: build run all vary_flags run clean debug

build: ${EXES}

run: ${CSVS}

all: run

vary_flags: ${reference_name}.x ${reference_name}_O1.x ${reference_name}_O2.x ${reference_name}_O3.x ${reference_name}_O3_native.x ${reference_name}_Ofast_native.x

clean:
	rm *.x *.csv

debug: CFLAGS+=-g
debug: all

%.csv: %.x
	bash run.sh $< ${RUN_REPEATS}

%.x: %.cpp
	${COMPILER} ${CFLAGS} ${OFLAGS} $< -o $@ ${LFLAGS}

${reference_name}_O1.x: ${reference_name}.cpp
	${COMPILER} ${CFLAGS} -O1 $< -o $@ ${LFLAGS}

${reference_name}_O2.x: ${reference_name}.cpp
	${COMPILER} ${CFLAGS} -O2 $< -o $@ ${LFLAGS}

${reference_name}_O3.x: ${reference_name}.cpp
	${COMPILER} ${CFLAGS} -O3 $< -o $@ ${LFLAGS}

${reference_name}_O3_native.x: ${reference_name}.cpp
	${COMPILER} ${CFLAGS} -O3 -march=native $< -o $@ ${LFLAGS}

${reference_name}_Ofast_native.x: ${reference_name}.cpp
	${COMPILER} ${CFLAGS} -Ofast -march=native $< -o $@ ${LFLAGS}
