#include "buf.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOVERLAP(buf, b, n)                                                    \
	if (b + n >= (buf)->buf && b < (buf)->last) {                              \
		errno = EINVAL;                                                        \
		return -1;                                                             \
	}

#define NOVERFLOW(buf, n)                                                      \
	if ((buf)->next_write + n > (buf)->last) {                                 \
		errno = EINVAL;                                                        \
		return -1;                                                             \
	}

inline void buf_reset_offsets(buf_t* buf)
{
	buf->next_write = buf->buf;
	buf->next_read = buf->buf;
}

void buf_init_values(buf_t* buf, char* cbuf, size_t cap)
{
	buf->buf = cbuf;
	buf->init_cap = cap;
	buf->cap = cap;
	buf->last = cbuf + cap;
	buf_reset_offsets(buf);
}

int buf_init(buf_t* buf, size_t cap)
{
	char* cbuf;

	if (buf == NULL) {
		errno = EINVAL;
		return -1;
	}

	if ((cbuf = malloc(cap * sizeof(char))) == NULL)
		return -1;

	buf_init_values(buf, cbuf, cap);

	return 0;
}

buf_t* buf_create(size_t cap)
{
	buf_t* buf;

	if ((buf = calloc(1, sizeof(buf_t))) == NULL)
		return NULL;

	if (buf_init(buf, cap) == -1) {
		free(buf);
		return NULL;
	}

	return buf;
}

void buf_reset(buf_t* buf)
{
	if (buf->cap > buf->init_cap && (buf->buf = realloc(buf->buf, buf->init_cap)) == NULL) {
		perror("realloc");
		abort();
	}

	buf_init_values(buf, buf->buf, buf->init_cap);
}

int buf_reserve(buf_t* buf, size_t additional)
{
	size_t read_offset = buf->next_read - buf->buf;
	size_t write_offset = buf->next_write - buf->buf;

	if ((buf->buf = realloc(buf->buf, buf->cap + additional)) == NULL)
		return -1;

	buf->next_write = buf->buf + write_offset;
	buf->next_read = buf->buf + read_offset;
	buf->cap += additional;
	buf->last = buf->buf + buf->cap;

	return 0;
}

int buf_write(buf_t* buf, const char* b, int n)
{
	if (n < 0) {
		errno = EINVAL;
		return -1;
	}

	NOVERFLOW(buf, n);
	NOVERLAP(buf, b, n);

	buf->next_write = memcpy(buf->next_write, b, n);

	return n;
}

int buf_read(const buf_t* buf, char* b, int n)
{
	int read;

	if (n < 0) {
		errno = EINVAL;
		return -1;
	}

	NOVERLAP(buf, b, n);
	read = buf_readable(buf);
	read = n > read ? read : n;

	memcpy(b, buf->next_read, read);

	return read;
}

void buf_free(buf_t* buf)
{
	if (buf != NULL && buf->buf != NULL) {
		free(buf->buf);
		free(buf);
	}
}

bool buf_compact(buf_t* buf)
{
	if (buf->next_read == buf->buf)
		return 0;

	int len = buf_readable(buf);
	memmove(buf->buf, buf->next_read, len);
	buf->next_write = buf->buf + len;
	buf->next_read = buf->buf;

	return 1;
}
