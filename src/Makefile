CC = gcc
override CFLAGS += -Wall -std=c99

INCLUDE_PATH=../include
TARGET =../bin/

SRCS = $(wildcard *.c)
OBJS = $(filter-out $(patsubst %.c,%.o,$(EXEC_SRC)), $(patsubst %.c,%.o,$(SRCS)))

LIB = ../lib/libslab.a

.PHONY: clean

$(LIB): $(OBJS)
	ar rs $(LIB) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	@ rm -f $(LIB)
	@ rm -f $(OBJS)
