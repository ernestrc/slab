.PHONY: test-full test analysis fuzz optimized format tags

TARGET=./bin
LIB=./lib

src: prepare
	@ cd src && CFLAGS='-ggdb -Werror' $(MAKE)

test-full: analysis test fuzz

prepare:
	@ mkdir -p $(LIB)
	@ mkdir -p $(TARGET)

clean:
	@ rm -rf $(TARGET) $(LIB)
	@ cd src && $(MAKE) $@
	@ cd test && $(MAKE) $@

optimized: prepare
	@ cd src && CFLAGS='-O2' $(MAKE)

# run clang-analyzer
analysis: clean
	@ scan-build $(MAKE)

test: src
	@ cd test && $(MAKE) test

fuzz: src
	@ cd test && $(MAKE) fuzz

format:
	@ find src -name \*.h -o -name \*.c | xargs clang-format -i
	@ find test -name \*.h -o -name \*.c | xargs clang-format -i

tags:
	@ ctags -R
