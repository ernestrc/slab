CC=clang
CFLAGS = -Wall -Werror -fsanitize=undefined -std=c11 -ggdb
TESTS=$(wildcard *.c)
BCODE=$(patsubst %.c,%.bc,$(TESTS)) $(patsubst %_klee.c, $(SRC)/%.bc, $(TESTS))
EXECS=$(patsubst %.c,%_out/test,$(TESTS))
CASES=$(patsubst %.c,%_out,$(TESTS))

INCLUDE=-I ~/klee_src/include -I /include
SRC=/include/slab
LIBKLEE=~/klee_build/klee/lib/
LIBSLAB=../../lib/libslab.a

.PHONY: test

test: $(BCODE) $(CASES) $(EXECS)
	@echo
	@echo "RUNNING SYMBOLIC TESTS"
	@echo "---------------------------"
	@for t in $(CASES); do cd $$t; for c in *.ktest; do echo "\nRUNNING $$t $$c"; ktest-tool --write-ints $$c && LD_LIBRARY_PATH=~/klee_build/klee/lib/:$$LD_LIBRARY_PATH KTEST_FILE=$$c ./test; done; done

%.bc: %.c
	@$(CC) $(CFLAGS) $(INCLUDE) -emit-llvm -c -g $<

%_out: %.c FORCE
	@-rm -rf $@
	@llvm-link $(patsubst %_klee.c,%_klee.bc,$<) $(patsubst %_klee.c,%.bc,$<) -o $(patsubst %.c,%.o,$<)
	klee --optimize --libc=uclibc --posix-runtime -output-dir=$@ $(patsubst %.c,%.o,$<)

%_out/test: %.c $(CASES)
	@$(CC) $(CFLAGS) $(INCLUDE) -L $(LIBKLEE) $< $(SRC)/$(patsubst %_klee.c,%.c,$<) -lkleeRuntest -o $@

clean:
	@rm -rf klee-last $(EXECS) $(CASES)
	@rm -rf klee-out-* *.bc

FORCE:
