CC = clang
CFLAGS = -O2 -Wall -std=c11

INCLUDE_PATH=../include
TARGET =../bin/

SRCS = $(wildcard *.c)
OBJS = $(filter-out $(patsubst %.c,%.o,$(EXEC_SRC)), $(patsubst %.c,%.o,$(SRCS)))

LIB = ../lib/libslab.a

.PHONY: clean

$(LIB): $(OBJS)
	ar rs $(LIB) $(OBJS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	@ rm -f $(LIB)
	@ rm -f $(OBJS)
