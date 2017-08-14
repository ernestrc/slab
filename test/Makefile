CC = gcc
override CFLAGS += -Wall -Werror -std=c99 -ggdb

TARGET = ../bin/
SRCS = $(wildcard *.c)
TESTS = $(addprefix $(TARGET),$(patsubst %.c,%,$(SRCS)))
LIB = ../lib/libslab.a

.PHONY: clean test

test: $(TESTS)
	@ for f in $^; do ./$$f; done

$(TARGET)%: %.c $(LIB)
	@$(CC) $(CFLAGS) $< -o $@ $(LIB)
	@$(CC) $(CFLAGS) -I./ $< -o $@ $(LIB)

clean:
	@rm -f $(TESTS)
