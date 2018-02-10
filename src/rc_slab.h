#ifndef LIBSLAB_RC_SLAB_H
#define LIBSLAB_RC_SLAB_H

#include "slab.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rc_slab_s {
	slab_t slab;
} rc_slab_t;

typedef struct rc_slot_s {
	int ref;
} rc_slot_t;

rc_slab_t* rc_slab_create(size_t cap, size_t slot_size);

int rc_slab_init(rc_slab_t* rc_slab, size_t cap, size_t slot_size);

void* rc_slab_get(rc_slab_t* rc_slab);

int rc_slab_put(rc_slab_t*, void* slot);

void rc_slab_deinit(rc_slab_t* rc_slab);

void rc_slab_free(rc_slab_t* rc_slab);

void* rc_slab_dup(void* slot);

#ifdef __cplusplus
}
#endif
#endif
