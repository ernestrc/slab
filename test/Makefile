CC = gcc
override CFLAGS += -Wall -fsanitize=address -Werror -std=c99 -ggdb

TARGET = ../bin/
TESTS = $(addprefix $(TARGET),$(patsubst %.c,%,$(wildcard *_test.c)))
FUZZERS = $(addprefix $(TARGET),$(patsubst %.c,%,$(wildcard *_fuzz.c)))
LIB = ../lib/libslab.a
LIBFUZZER=./libFuzzer.a
SRCFUZZER=./Fuzzer

.PHONY: clean test fuzz

test: $(TESTS)
	@ for f in $^; do ./$$f; done

$(TARGET)%test: %test.c $(LIB)
	@$(CC) $(CFLAGS) -I./ $< -o $@ $(LIB)

clean:
	@rm -f $(TESTS) $(FUZZERS)
	@rm -rf $(SRCFUZZER) $(LIBFUZZER)

fuzz: $(FUZZERS)
	@ for f in $^; do echo $$f; done

$(TARGET)%fuzz: %fuzz.c $(LIBFUZZER)
	clang -D_GLIBCXX_USE_CXX11_ABI=0 -fsanitize-coverage=trace-pc-guard -fsanitize=address,undefined $(addprefix ../src/,$(patsubst %_fuzz.c, %.c, $<)) $< $(LIBFUZZER) -o $@

$(LIBFUZZER): $(SRCFUZZER)
	@$(SRCFUZZER)/build.sh

$(SRCFUZZER):
	@git clone https://chromium.googlesource.com/chromium/llvm-project/llvm/lib/Fuzzer
