.PHONY: test analysis fuzz dist format tags coverage full-coverage fuzz-coverage fuzz-full-coverage

TARGET=./bin
LIB=./lib
TEST=./test
SRC=./src

TEST_FLAGS=CC=clang CFLAGS='-Wall -Werror -fsanitize=undefined -fsanitize-coverage=trace-cmp,trace-pc-guard -fprofile-instr-generate -fcoverage-mapping -std=c11 -ggdb'

src: prepare
	@ cd $(SRC) && $(MAKE)

prepare:
	@ mkdir -p $(LIB)
	@ mkdir -p $(TARGET)

clean:
	@ rm -rf $(TARGET) $(LIB)
	@ cd $(SRC) && $(MAKE) $@
	@ cd $(TEST) && $(MAKE) $@

# run clang-analyzer
analysis: clean
	@ scan-build $(MAKE)

test: clean prepare
	@ cd $(SRC) && $(MAKE) $(TEST_FLAGS)
	@ cd $(TEST) && $(MAKE) $(TEST_FLAGS) test

fuzz: src
	@ cd $(TEST) && $(MAKE) $@

coverage:
	cd $(TEST) && $(MAKE) $@

full-coverage:
	cd $(TEST) && $(MAKE) $@

fuzz-coverage:
	cd $(TEST) && $(MAKE) $@

fuzz-full-coverage:
	cd $(TEST) && $(MAKE) $@

format:
	@ find $(SRC) -name \*.h -o -name \*.c | xargs clang-format -i
	@ find $(TEST) -name \*.h -o -name \*.c | xargs clang-format -i

tags:
	@ ctags -R
