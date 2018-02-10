#ifndef LIBSLAB_SLAB_H
#define LIBSLAB_SLAB_H

#include <stdlib.h>
#include <stddef.h>
#include <stdalign.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct slab_slot_s {
	struct slab_slot_s* next;
} slab_slot_t;

typedef struct slab_s {
	/* next available slot */
	slab_slot_t* next;
	/* blocks of slab slots */
	slab_slot_t** blocks;
	/* allocated block sizes */
	size_t* block_size;
	/* slot size = header + object size */
	size_t slot_size;
	/* total slab capacity */
	size_t cap;
	/* number of blocks allocated */
	int block_n;
} slab_t;

slab_t* slab_create(size_t cap, size_t slot_size);

int slab_init(slab_t* slab, size_t cap, size_t slot_size);

void* slab_get(slab_t*);

int slab_put(slab_t*, void* slot);

int slab_reserve(slab_t*, size_t additional);

void slab_deinit(slab_t*);

void slab_free(slab_t*);

char __HEADER_OFFSET;

#define __SLAB_SLOT_SIZE(size) (alignof(max_align_t) + (size))

#define __SLAB_SLOT_VALUE(tp, slot) ((void*)((char*)slot + __HEADER_OFFSET))

#define __SLAB_SLOT_HEADER(tp, value)                                          \
	((tp*)(((char*)(value)) - __HEADER_OFFSET))

#ifdef __cplusplus
}
#endif
#endif
