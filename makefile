PRECISION=double
CFLAGS=-Wall -Wextra -DPRECISION=${PRECISION}
LFLAGS=-lm
OFLAGS=-O3 -march=native
reference_name=$(basename ${shell ./ls_latest.sh})

EXES=$(subst .c,.x,$(shell ls v*.c))

build: ${EXES}

run: build
	bash run_benchmarks.sh | sort -nk5 | column -t -s' '

all: run

vary_flags: ${reference_name}.x ${reference_name}_O0.x ${reference_name}_O1.x ${reference_name}_O2.x ${reference_name}_O3.x ${reference_name}_O3_native.x ${reference_name}_Ofast_native.x

clean:
	rm *.x

debug: CFLAGS+=-g
debug: all

%.x: %.c
	gcc ${CFLAGS} ${OFLAGS} $< -o $@ ${LFLAGS}

${reference_name}_O0.x: ${reference_name}.c
	gcc ${CFLAGS} -O0 $< -o $@ ${LFLAGS}

${reference_name}_O1.x: ${reference_name}.c
	gcc ${CFLAGS} -O1 $< -o $@ ${LFLAGS}

${reference_name}_O2.x: ${reference_name}.c
	gcc ${CFLAGS} -O2 $< -o $@ ${LFLAGS}

${reference_name}_O3.x: ${reference_name}.c
	gcc ${CFLAGS} -O3 $< -o $@ ${LFLAGS}

${reference_name}_O3_native.x: ${reference_name}.c
	gcc ${CFLAGS} -O3 -march=native $< -o $@ ${LFLAGS}

${reference_name}_Ofast_native.x: ${reference_name}.c
	gcc ${CFLAGS} -Ofast -march=native $< -o $@ ${LFLAGS}
