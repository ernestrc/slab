#include "./circ_array.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN_BLOCK_SIZE 16
#define BLOCKIZE(cap) (((cap) / MIN_BLOCK_SIZE) + 1) * MIN_BLOCK_SIZE

circ_array_t* circ_array_create(size_t init_cap)
{
	circ_array_t* res = NULL;
	if ((res = calloc(1, sizeof(circ_array_t))) == NULL) {
		return NULL;
	}

	if (circ_array_init(res, init_cap) != 0) {
		int _errno = errno;
		free(res);
		errno = _errno;
		return NULL;
	}

	return res;
}

int circ_array_init(circ_array_t* a, size_t cap)
{
	if (a == NULL || cap < 0) {
		errno = EINVAL;
		return -1;
	}

	cap = BLOCKIZE(cap);

	if ((a->a = realloc(a->a, cap * sizeof(void*))) == NULL) {
		return -1;
	}

	a->cap = cap;
	a->len = 0;

	return 0;
}

int circ_array_resize(circ_array_t* a, int from, int to)
{
	if (to < from) {
		abort();
	}
	int len = a->len;
	int expected_cap = BLOCKIZE(to - from);

	if (expected_cap != a->cap) {
		if (circ_array_init(a, expected_cap) != 0) {
			return -1;
		}
		a->len = len;
	}

	return 0;
}

void* circ_array_get(circ_array_t* a, int idx) {
	if (idx < a->first || idx > a->last) {
		errno = EINVAL;
		return NULL;
	}
	return a->a[idx % a->len];
}

// TODO
void* circ_array_pop_last(circ_array_t* a)
{
	int new_len = a->len - 1;
	void* el = circ_array_get(a, new_len);
}

	// TODO
void* circ_array_pop_first(circ_array_t* a)
{
	void* el = circ_array_get(a, 0);
}

	// TODO
void circ_array_put(circ_array_t* a, int idx, void* o)
{
}

// TODO
void circ_array_append(circ_array_t* a, void* o)
{
	if (circ_array_resize(a, 0, a->len) != 0) {
		perror("circ_array_resize");
		abort();
	}
	circ_array_put(a, a->len, o);
}

// TODO
void circ_array_prepend(circ_array_t* a, void* o)
{
	if (circ_array_resize(a, 0, a->len) != 0) {
		perror("circ_array_resize");
		abort();
	}
	circ_array_put(a, 0, o);
}

void circ_array_deinit(circ_array_t* a)
{
	if (a && a->a) {
		free(a->a);
		a->a = NULL;
	}
}

void circ_array_free(circ_array_t* a)
{
	if (a) {
		circ_array_deinit(a);
		free(a);
	}
}
