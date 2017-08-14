.PHONY: clean test purge analysis optimized format tags

TARGET=./bin
LIB=./lib

src: prepare
	@ cd src && CFLAGS='-ggdb -Werror' $(MAKE)

test: src prepare
	@ cd test && $(MAKE) $@

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
	scan-build $(MAKE) -s

format:
	@ find src -name \*.h -o -name \*.c | xargs clang-format -i
	@ find test -name \*.h -o -name \*.c | xargs clang-format -i

tags:
	@ ctags -R
