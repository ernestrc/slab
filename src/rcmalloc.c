#include <dlfcn.h>
#include <errno.h>
#include <malloc.h>
#include <stdio.h>

#include "rc_slab.h"
#include "rcmalloc.h"

#define MALLOC_SIZE0 256
#define MALLOC_SIZE1 4096
#define MALLOC_SIZE2 16384
#define MALLOC_SIZE3 65536

static rc_slab_t SLAB_SIZE0;
static rc_slab_t SLAB_SIZE1;
static rc_slab_t SLAB_SIZE2;
static rc_slab_t SLAB_SIZE3;

typedef struct rc_buf_s {
	size_t size;
} rc_buf_t;

#define RCB_SLOT_VALUE(slot) ((void*)((rc_buf_t*)slot + 1))

#define RCB_SLOT_HEADER(slot) (((rc_buf_t*)(slot)) - 1)

void* rcmalloc(size_t size)
{
	rc_buf_t* slot;

	if (size < MALLOC_SIZE0) {
		slot = rc_slab_get(&SLAB_SIZE0);
		size = MALLOC_SIZE0;
	} else if (size < MALLOC_SIZE1) {
		slot = rc_slab_get(&SLAB_SIZE1);
		size = MALLOC_SIZE1;
	} else if (size < MALLOC_SIZE2) {
		slot = rc_slab_get(&SLAB_SIZE2);
		size = MALLOC_SIZE2;
	} else if (size < MALLOC_SIZE3) {
		slot = rc_slab_get(&SLAB_SIZE3);
		size = MALLOC_SIZE3;
	} else {
		errno = ENOMEM;
		return NULL;
	}

	if (slot == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	slot->size = size;

	return RCB_SLOT_VALUE(slot);
}

void rcfree(void* ptr)
{
	rc_buf_t* slot = RCB_SLOT_HEADER(ptr);

	switch (slot->size) {
	case MALLOC_SIZE0:
		rc_slab_put(&SLAB_SIZE0, slot);
		break;
	case MALLOC_SIZE1:
		rc_slab_put(&SLAB_SIZE1, slot);
		break;
	case MALLOC_SIZE2:
		rc_slab_put(&SLAB_SIZE2, slot);
		break;
	case MALLOC_SIZE3:
		rc_slab_put(&SLAB_SIZE3, slot);
		break;
	}
}

int rcmalloc_init(size_t cap)
{
	if (rc_slab_init(&SLAB_SIZE0, cap, MALLOC_SIZE0) == -1)
		goto err;

	if ((rc_slab_init(&SLAB_SIZE1, cap, MALLOC_SIZE1)) == -1)
		goto err;

	if ((rc_slab_init(&SLAB_SIZE2, cap, MALLOC_SIZE2)) == -1)
		goto err;

	if ((rc_slab_init(&SLAB_SIZE3, cap, MALLOC_SIZE3)) == -1)
		goto err;

	return 0;

err:
	rcmalloc_deinit();
	return -1;
}

void* rcdup(void* ptr)
{
	rc_buf_t* slot = RCB_SLOT_HEADER(ptr);

	return RCB_SLOT_VALUE(rc_slab_dup(slot));
}

void rcmalloc_deinit()
{
	rc_slab_deinit(&SLAB_SIZE0);
	rc_slab_deinit(&SLAB_SIZE1);
	rc_slab_deinit(&SLAB_SIZE2);
	rc_slab_deinit(&SLAB_SIZE3);
}
