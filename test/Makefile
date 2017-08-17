CC = clang
# TODO renable address sanitizer
CFLAGS = -Wall -fprofile-instr-generate -fcoverage-mapping -Werror -std=c99 -ggdb

SRC = ../src
TARGET = ../bin
TESTS = $(addprefix $(TARGET)/,$(patsubst %.c,%,$(wildcard *_test.c)))
FUZZERS = $(addprefix $(TARGET)/,$(patsubst %.c,%,$(wildcard *_fuzz.c)))
TCOVERAGES = $(addprefix $(TARGET)/,$(patsubst %.c,%.prof,$(wildcard *_test.c)))
FCOVERAGES = $(addprefix $(TARGET)/,$(patsubst %.c,%.prof,$(wildcard *_fuzz.c)))

LIBSLAB = ../lib/libslab.a

TESTPROFDATA=$(TARGET)/tests.profdata
FUZZPROFDATA=$(TARGET)/fuzz.profdata

.PHONY: clean test fuzz coverage full-coverage fuzz-coverage

test: $(TESTS)
	@ for f in $^; do LLVM_PROFILE_FILE="$$f.prof" ./$$f; done

$(TARGET)/%_test: %_test.c $(SRC)/%.c
	$(CC) $(CFLAGS) -I./ $(addprefix ../src/,$(patsubst %_test.c, %.c, $<)) $< -o $@ $(LIBSLAB)

$(TESTPROFDATA): test
	llvm-profdata merge -sparse $(TCOVERAGES) -o $(TESTPROFDATA)

coverage: $(TESTPROFDATA)
	for f in $(TESTS); do llvm-cov report $$f -instr-profile=$(TESTPROFDATA); done

full-coverage: $(TESTPROFDATA)
	for f in $(TESTS); do llvm-cov show $$f -instr-profile=$(TESTPROFDATA); done

fuzz: $(FUZZERS)
	@ for f in $^; do mkdir -p $$f.corpus && LLVM_PROFILE_FILE="$$f.prof" $$f $$f.corpus & done; wait

# clang version >= April 2017 required (tested with 6.0.0 trunk 310227)
$(TARGET)/%_fuzz: %_fuzz.c $(SRC)/%.c
	$(CC) $(CFLAGS) -fsanitize=fuzzer $(addprefix ../src/,$(patsubst %_fuzz.c, %.c, $<)) $< -o $@ $(LIBSLAB)

$(FUZZPROFDATA):
	llvm-profdata merge -sparse $(FCOVERAGES) -o $(FUZZPROFDATA)

fuzz-coverage: $(FUZZPROFDATA)
	for f in $(FUZZERS); do llvm-cov report $$f -instr-profile=$(FUZZPROFDATA); done

fuzz-full-coverage: $(FUZZPROFDATA)
	for f in $(FUZZERS); do llvm-cov show $$f -instr-profile=$(FUZZPROFDATA); done

clean:
	@rm -f $(TESTS) $(FUZZERS) $(FUZZPROFDATA) $(TESTPROFDATA) $(TCOVERAGES) $(FCOVERAGES)

