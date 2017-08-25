#include "rc_slab.h"
#include <errno.h>

#define RC_SLAB_SLOT_VALUE(slot) __SLAB_SLOT_VALUE(rc_slot_t, slot)

#define RC_SLAB_SLOT_HEADER(value) __SLAB_SLOT_HEADER(rc_slot_t, value)

rc_slab_t* rc_slab_create(size_t cap, size_t slot_size)
{
	rc_slab_t* rc_slab;

	if ((rc_slab = calloc(1, sizeof(rc_slab_t))) == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	if (rc_slab_init(rc_slab, cap, slot_size) == -1) {
		free(rc_slab);
		return NULL;
	}

	return rc_slab;
}

int rc_slab_init(rc_slab_t* rc_slab, size_t cap, size_t slot_size)
{
	if (rc_slab == NULL || cap == 0) {
		errno = EINVAL;
		return -1;
	}

	if (slab_init(&rc_slab->slab, cap, __SLAB_SLOT_SIZE(slot_size)) == -1)
		return -1;

	return 0;
}

void rc_slab_deinit(rc_slab_t* rc_slab) { slab_deinit(&rc_slab->slab); }

void rc_slab_free(rc_slab_t* rc_slab)
{
	if (rc_slab == NULL)
		return;
	rc_slab_deinit(rc_slab);
	free(rc_slab);
}

void* rc_slab_get(rc_slab_t* rc_slab)
{
	rc_slot_t* slot;

	if ((slot = slab_get(&rc_slab->slab)) == NULL)
		return NULL;

	slot->ref = 1;

	return RC_SLAB_SLOT_VALUE(slot);
}

int rc_slab_put(rc_slab_t* rc_slab, void* slot_value)
{
	rc_slot_t* slot = RC_SLAB_SLOT_HEADER(slot_value);

	if (slot->ref <= 0) {
		errno = EINVAL;
		return -1;
	}

	slot->ref--;

	if (slot->ref == 0 && slab_put(&rc_slab->slab, slot) == -1)
		return -1;

	return 0;
}

void* rc_slab_dup(void* slot_value)
{
	rc_slot_t* slot = RC_SLAB_SLOT_HEADER(slot_value);

	if (slot->ref <= 0) {
		errno = EINVAL;
		return NULL;
	}

	slot->ref++;

	return slot_value;
}
