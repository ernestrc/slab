#ifndef LIBSLAB_SLAB_H
#define LIBSLAB_SLAB_H

#include <stdlib.h>

typedef struct slab_slot_s {
	struct slab_slot_s* next;
} slab_slot_t;

typedef struct slab_s {
	/* next available slot */
	slab_slot_t* next;
	/* blocks of slab slots */
	slab_slot_t** blocks;
	/* slot size = header + object size */
	size_t slot_size;
	/* allocated block sizes */
	size_t* block_size;
	/* number of blocks allocated */
	int block_n;
	/* total slab capacity */
	size_t cap;
} slab_t;

slab_t* slab_create(size_t cap, size_t slot_size);

int slab_init(slab_t* slab, size_t cap, size_t slot_size);

void* slab_get(slab_t*);

int slab_put(slab_t*, void* slot);

int slab_reserve(slab_t*, size_t additional);

void slab_deinit(slab_t*);

void slab_free(slab_t*);

#define SLAB_FOREACH(slab, name, code)                                         \
	{                                                                          \
		slab_slot_t** __blocks;                                                \
		size_t* __block_sizes;                                                 \
		slab_slot_t* __slot;                                                   \
		size_t __block_size;                                                   \
		int __n;                                                               \
                                                                               \
		for (__blocks = slab->blocks, __block_sizes = slab->block_size,        \
			__n = slab->block_n;                                               \
			 __n > 0; __n--, __blocks++, __block_sizes++) {                    \
			for (__slot = *__blocks, __block_size = *__block_sizes;            \
				 __block_size > 1;                                             \
				 __block_size--, __slot = SLAB_NEXT_SLOT(slab, __slot)) {      \
				(name) = SLAB_SLOT_VALUE(__slot);                              \
				code;                                                          \
			}                                                                  \
		}                                                                      \
	}

#endif
