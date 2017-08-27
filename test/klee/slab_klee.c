#include <stdlib.h>
#include <stdio.h>
#include <klee/klee.h>

#define LIBSLAB_AUTO 1

#include <slab/slab.h>

int main(int argc, char** argv) {
	size_t cap, size;
	int iterget, iterput;

	klee_make_symbolic(&cap, sizeof(cap), "cap");
	klee_make_symbolic(&iterget, sizeof(iterput), "iterput");
	klee_make_symbolic(&iterput, sizeof(iterget), "iterget");
	klee_make_symbolic(&size, sizeof(size), "size");

	slab_t slab;

	if (slab_init(&slab, cap, size) == -1)
		return 0;

	if (iterput > iterget)
		iterput = iterget;

	char* slots[iterget];

	for (int i = 0; i < iterget; i++) {
		slots[i] = slab_get(&slab);
	}

	for (int i = 0; i < iterput; i++) {
		slab_put(&slab, slots[i]);
	}

	slab_deinit(&slab);

	return 0;
}
