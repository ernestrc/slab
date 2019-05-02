#ifndef LIBSLAB_CIRC_ARRAY_H
#define LIBSLAB_CIRC_ARRAY_H

#include <stddef.h>

typedef struct circ_array_s {
	void** a;
	int first;
	int last;
	size_t len;
	size_t cap;
} circ_array_t;

circ_array_t* circ_array_create(size_t init_cap);

int circ_array_init(circ_array_t* a, size_t init_cap);

void* circ_array_get(circ_array_t* a, int idx);

void* circ_array_pop_last(circ_array_t* a);

void* circ_array_pop_first(circ_array_t* a);

void circ_array_put(circ_array_t* a, int idx, void* o);

void circ_array_append(circ_array_t* a, void* o);

void circ_array_prepend(circ_array_t* a, void* o);

int circ_array_resize(circ_array_t* a, int from, int to);

void circ_array_deinit(circ_array_t* a);

void circ_array_free(circ_array_t* a);

#endif
