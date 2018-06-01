#ifndef LIBSLAB_BUFFER_H
#define LIBSLAB_BUFFER_H

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#define buf_reset_offsets(b)                                                   \
	(b)->next_write = (b)->buf;                                                \
	(b)->next_read = (b)->buf;

void buf_reset(buf_t*);

int buf_reserve(buf_t*, size_t additional);

#define buf_readable(b) ((b)->next_write - (b)->next_read)

#define buf_writable(b) ((b)->last - (b)->next_write)

#define buf_full(b) ((b)->last == (b)->next_write)

int buf_write(buf_t*, const char* b, int n);

int buf_read(const buf_t*, char* b, int n);

/* Attempts to compact buffer on the read side
 * by memmoving relevant data. Returns true if compacting succeeds. */
bool buf_compact(buf_t*);

/* Free this buffer and all its associated resources. Use only if buffer
 * was allocated using buf_create */
void buf_free(buf_t*);

/* Moves write offset by n bytes */
#define buf_extend(b, n) (b)->next_write += n;

/* Moves read offset by n bytes */
#define buf_ack(b, n) (b)->next_read += n;

/* Attempts to compact buffer by reseting offsets */
#define buf_ecompact(b)                                                        \
	if ((b)->next_read == (b)->next_write) {                                   \
		buf_reset_offsets(b);                                                  \
	}

/* Moves read offset by n bytes and attempts to reset offsets */
#define buf_consume(b, n)                                                      \
	buf_ack(b, n);                                                             \
	buf_ecompact(b);

#ifdef __cplusplus
}
#endif
#endif
