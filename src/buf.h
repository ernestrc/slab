#ifndef LIBSLAB_BUFFER_H
#define LIBSLAB_BUFFER_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
	size_t init_cap;
	size_t cap;
	char* buf;
	char* next_write;
	char* next_read;
	char* last;
} buf_t;

buf_t* buf_create(size_t cap);

int buf_init(buf_t* buf, size_t cap);

void buf_init_values(buf_t* buf, char* cbuf, size_t cap);

void buf_reset_offsets(buf_t*);

void buf_reset(buf_t*);

int buf_reserve(buf_t*, size_t additional);

#define buf_readable(b) ((b)->next_write - (b)->next_read)

#define buf_writable(b) ((b)->last - (b)->next_write)

int buf_write(buf_t*, const char* b, int n);

int buf_read(const buf_t*, char* b, int n);

bool buf_compact(buf_t*);

void buf_free(buf_t*);

#define buf_extend(buf, n) (buf)->next_write += n;

#define buf_consume(buf, n)                                                    \
	(buf)->next_read += n;                                                     \
	if ((buf)->next_read == (buf)->next_write) {                               \
		buf_reset_offsets(buf);                                                \
	}

#endif
