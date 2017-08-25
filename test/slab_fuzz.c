#include <stdint.h>

#include "../src/rc_slab.h"
#include "test_aligned.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	rc_slab_t* slab;
	size_t struct_size;
	size_t struct_alignment;

	if (data == NULL || size <= 0)
		return 0;

	switch (*data % 7) {
		case 0:
			struct_size = sizeof(struct one);
			struct_alignment = alignof(struct one);
			break;
		case 1:
			struct_size = sizeof(struct two);
			struct_alignment = alignof(struct two);
			break;
		case 2:
			struct_size = sizeof(struct three);
			struct_alignment = alignof(struct three);
			break;
		case 3:
			struct_size = sizeof(struct four);
			struct_alignment = alignof(struct four);
			break;
		case 4:
			struct_size = sizeof(struct five);
			struct_alignment = alignof(struct five);
			break;
		case 5:
			struct_size = sizeof(struct six);
			struct_alignment = alignof(struct six);
			break;
		default:
			struct_size = sizeof(struct seven);
			struct_alignment = alignof(struct seven);
			break;
	}

	if ((slab = rc_slab_create(size, struct_size)) == NULL)
		abort();

	// improve coverage
#ifdef LIBSLAB_AUTO
	size *= 2;
#endif

	void** slots = calloc(size, sizeof(void*));
	void** slot = slots;
	size_t i;

	for (i = 0; i < size; i++, slot++) {
		*slot = rc_slab_get(slab);
		// test alignment
		if((((uintptr_t)(*slot)) & ((uintptr_t)(struct_alignment - 1))))
			abort();
		rc_slab_dup(*slot);
	}

	for (slot = slots, i = 0; i < size; i++, slot++) {
		rc_slab_put(slab, *slot);
		*slot = rc_slab_dup(*slot);
		rc_slab_put(slab, *slot);
		rc_slab_put(slab, *slot);
	}

	rc_slab_free(slab);

	return 0;
}
