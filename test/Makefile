CC = clang
CFLAGS = -Wall -Werror -fsanitize-coverage=trace-cmp -fsanitize-coverage=trace-pc-guard -fprofile-instr-generate -fcoverage-mapping -std=c99 -g

SRC = ../src
TARGET = ../bin
TESTS = $(addprefix $(TARGET)/,$(patsubst %.c,%,$(wildcard *_test.c)))
FUZZERS = $(addprefix $(TARGET)/,$(patsubst %.c,%,$(wildcard *_fuzz.c)))
TPROFILES = $(addprefix $(TARGET)/,$(patsubst %.c,%.profraw,$(wildcard *_test.c)))
FPROFILES = $(addprefix $(TARGET)/,$(patsubst %.c,%.profraw,$(wildcard *_fuzz.c)))

LIBSLAB = ../lib/libslab.a

TESTPROFDATA=$(TARGET)/tests.profdata
FUZZPROFDATA=$(TARGET)/fuzz.profdata

.PHONY: clean test fuzz coverage full-coverage fuzz-coverage

test: $(TESTS)
	@ for f in $^; do LLVM_PROFILE_FILE="$$f.profraw" ./$$f; done

$(TARGET)/%_test: %_test.c $(SRC)/%.c
	$(CC) $(CFLAGS) -I./ $(addprefix ../src/,$(patsubst %_test.c, %.c, $<)) $< -o $@ $(LIBSLAB)

$(TESTPROFDATA): $(TESTS)
	llvm-profdata merge -sparse $(TPROFILES) -o $(TESTPROFDATA)

coverage: $(TESTPROFDATA)
	llvm-cov report -instr-profile=$(TESTPROFDATA) $(addprefix -object ,$(TESTS))

full-coverage: $(TESTPROFDATA) BROWSER
	llvm-cov show -format=html -instr-profile=$(TESTPROFDATA) $(addprefix -object ,$(TESTS)) > $(TARGET)/coverage.html
	@$(BROWSER) $(TARGET)/coverage.html

fuzz: $(FUZZERS)
	@ for f in $^; do mkdir -p $$f.corpus && LLVM_PROFILE_FILE="$$f.profraw" $$f $$f.corpus -runs=100000000; done

# clang version >= April 2017 required (tested with 6.0.0 trunk 310227)
$(TARGET)/%_fuzz: %_fuzz.c $(SRC)/%.c
	$(CC) $(CFLAGS) -fsanitize=fuzzer $(addprefix ../src/,$(patsubst %_fuzz.c, %.c, $<)) $< -o $@ $(LIBSLAB)

$(FUZZPROFDATA): $(FUZZERS)
	llvm-profdata merge -sparse $(FPROFILES) -o $(FUZZPROFDATA)

fuzz-coverage: $(FUZZPROFDATA)
	llvm-cov report -instr-profile=$(FUZZPROFDATA) $(addprefix -object ,$(FUZZERS))

fuzz-full-coverage: $(FUZZPROFDATA) BROWSER
	llvm-cov show -format=html -instr-profile=$(FUZZPROFDATA) $(addprefix -object ,$(FUZZERS)) > $(TARGET)/fuzz-coverage.html
	@$(BROWSER) $(TARGET)/fuzz-coverage.html

clean:
	@rm -f $(TESTS) $(FUZZERS) $(FUZZPROFDATA) $(TESTPROFDATA) $(TPROFILES) $(FPROFILES)

BROWSER:
ifndef BROWSER
	$(error BROWSER env variable is undefined)
endif
