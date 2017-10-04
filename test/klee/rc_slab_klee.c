#include <stdlib.h>
#include <stdio.h>
#include <klee/klee.h>

#include <slab/rc_slab.h>

int main(int argc, char** argv) {
	size_t cap, size;
	int iterget, iterput, iterdup;

	klee_make_symbolic(&cap, sizeof(cap), "cap");
	klee_make_symbolic(&iterget, sizeof(iterget), "iterget");
	klee_make_symbolic(&iterdup, sizeof(iterdup), "iterdup");
	klee_make_symbolic(&iterput, sizeof(iterput), "iterput");
	klee_make_symbolic(&size, sizeof(size), "size");

	rc_slab_t slab;

	if (rc_slab_init(&slab, cap, size) == -1)
		return 0;

	if (iterput > iterget)
		iterput = iterget;

	char* slots[iterget];

	for (int i = 0; i < iterget; i++) {
		slots[i] = rc_slab_get(&slab);
	}

	for (int i = 0; i < iterdup; i++) {
		rc_slab_dup(slots[i]);
	}

	for (int i = 0; i < iterput; i++) {
		rc_slab_put(&slab, slots[i]);
	}

	rc_slab_deinit(&slab);

	return 0;
}
