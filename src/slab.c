#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "slab.h"

#define SLAB_NEXT_SLOT(slab, block)                                            \
	(slab_slot_t*)(((void*)block) + slab->slot_size)

#define SLAB_SLOT_VALUE(slot) ((void*)((slab_slot_t*)slot + 1))

#define SLAB_SLOT_HEADER(slot) (((slab_slot_t*)(slot)) - 1)

static slab_slot_t* slab_link_slots(
  slab_t* slab, slab_slot_t* block, size_t block_size)
{
	slab_slot_t* next;

	if (block_size == 0) {
		errno = EINVAL;
		return NULL;
	}

	for (; block_size > 1; block_size--, block = next) {
		next = SLAB_NEXT_SLOT(slab, block);
		block->next = next;
	};

	block->next = NULL;

	return block;
}

slab_t* slab_create(size_t cap, size_t slot_size)
{
	slab_t* slab;

	if ((slab = calloc(1, sizeof(slab_t))) == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	if (slab_init(slab, cap, slot_size) == -1) {
		free(slab);
		return NULL;
	}

	return slab;
}

int slab_init(slab_t* slab, size_t cap, size_t slot_size)
{
	if (slab == NULL || cap == 0) {
		errno = EINVAL;
		return -1;
	}

	if ((slab->blocks = calloc(1, sizeof(void*))) == NULL) {
		errno = ENOMEM;
		goto err;
	}

	if ((slab->block_size = calloc(1, sizeof(size_t))) == NULL) {
		errno = ENOMEM;
		goto err;
	}

	slab->slot_size = sizeof(slab_slot_t) + slot_size;

	if ((*slab->blocks = calloc(cap, slab->slot_size)) == NULL) {
		errno = ENOMEM;
		goto err;
	}

	*slab->block_size = cap;
	slab->block_n = 1;
	slab->next = *slab->blocks;
	slab->cap = cap;

	if (slab_link_slots(slab, *slab->blocks, cap) == NULL)
		goto err;

	return 0;

err:
	if (slab->blocks) {
		if (*slab->blocks)
			free(*slab->blocks);
		free(slab->blocks);
	}
	if (slab->block_size)
		free(slab->block_size);
	return -1;
}

int slab_reserve(slab_t* slab, size_t additional)
{
	slab_slot_t* last_slot;
	int block_n;
	int block_i;

	if (additional == 0) {
		errno = EINVAL;
		return -1;
	}

	block_i = slab->block_n;
	block_n = slab->block_n + 1;

	if ((slab->block_size =
			realloc(slab->block_size, block_n * sizeof(size_t))) == NULL)
		return -1;

	if ((slab->blocks = realloc(slab->blocks, block_n * sizeof(void*))) == NULL)
		return -1;

	if ((slab->blocks[block_i] = calloc(additional, slab->slot_size)) == NULL)
		return -1;

	slab->block_size[block_i] = additional;

	if ((last_slot =
			slab_link_slots(slab, slab->blocks[block_i], additional)) == NULL) {
		free(slab->blocks[block_i]);
		return -1;
	}

	last_slot->next = slab->next;
	slab->next = slab->blocks[block_i];

	slab->block_n = block_n;
	slab->cap += additional;

	return 0;
}

void slab_deinit(slab_t* slab)
{
	slab_slot_t** blocks = slab->blocks;
	size_t* size = slab->block_size;
	int n = slab->block_n;

	for (; n > 0; n--, blocks++, size++)
		free(*blocks);

	free(slab->blocks);
	free(slab->block_size);
}

void slab_free(slab_t* slab)
{
	if (slab == NULL)
		return;

	slab_deinit(slab);
	free(slab);
}

int slab_put(slab_t* slab, void* slot_value)
{
	slab_slot_t* slot;

	if (slot_value == NULL) {
		errno = EINVAL;
		return -1;
	}

	slot = SLAB_SLOT_HEADER(slot_value);

	slab_slot_t* next = slab->next;

	slab->next = slot;
	slab->next->next = next;

	return 0;
}

void* slab_get(slab_t* slab)
{
#ifndef LIBSLAB_AUTO
	if (slab->next == NULL) {
		errno = ENOMEM;
		return NULL;
	}
#else
	if (slab->next == NULL && slab_reserve(slab, slab->cap)) {
		perror("slab_reserve");
		return NULL;
	}
#endif

	slab_slot_t* next = slab->next;
	slab->next = next->next;
	next->next = NULL;

	return SLAB_SLOT_VALUE(next);
}
