#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../src/slab.h"
#include "test.h"
#include "test_aligned.h"

typedef struct test_struct_s {
	int val1;
	int val2;
} test_struct;

size_t BUF_CAP = 16;

int test_slab_free_null()
{
	slab_free(NULL);
	return 0;
}

int test_slab_create()
{
	size_t size = 4;
	slab_t* b = slab_create(size, sizeof(test_struct));

	ASSERT_NEQ(b, NULL);
	ASSERT_NEQ(b->blocks, NULL);

	ASSERT_NEQ(&b->blocks[0][0], NULL);
	ASSERT_NEQ(&b->blocks[0][1], NULL);
	ASSERT_NEQ(&b->blocks[0][2], NULL);
	ASSERT_NEQ(&b->blocks[0][3], NULL);

	slab_slot_t* block = *b->blocks;

	ASSERT_EQ(b->next, block);
	ASSERT_EQ(b->next->next, ((void*)block) + b->slot_size * 1);
	ASSERT_EQ(b->next->next->next, ((void*)block) + b->slot_size * 2);
	ASSERT_EQ(b->next->next->next->next, ((void*)block) + b->slot_size * 3);
	ASSERT_EQ(b->next->next->next->next->next, NULL);

	slab_free(b);

	return 0;
}

int test_slab_get()
{
	size_t size = 512;
	slab_t* b = slab_create(size, sizeof(test_struct));

	test_struct* value;
	size_t i;

	for (i = 0; i < size; ++i) {
		value = slab_get(b);
		ASSERT_NEQ(value, NULL);
		ASSERT_ALIGNED(value, test_struct);
	}

	ASSERT_NULL(slab_get(b));
	slab_free(b);

	return 0;
}

int test_slab_resize()
{
	size_t i;

	size_t init_cap = 8;
	size_t final_cap = 24;
	test_struct** s = calloc(final_cap, sizeof(test_struct*));
	test_struct** slot = s;

	slab_t* slab = slab_create(init_cap, sizeof(test_struct));

	for (i = 0; i < init_cap; i++, slot++) {
		*slot = slab_get(slab);
		ASSERT_NEQ(*slot, NULL);
		ASSERT_ALIGNED(*slot, test_struct);
		(*slot)->val1 = i;
	}

	ASSERT_EQ(slab_get(slab), NULL);
	ASSERT_EQ(slab_reserve(slab, 4), 0);
	ASSERT_EQ(slab_reserve(slab, 4), 0);
	ASSERT_EQ(slab_reserve(slab, 8), 0);

	for (i = init_cap; i < final_cap; i++, slot++) {
		*slot = slab_get(slab);
		ASSERT_NEQ(*slot, NULL);
		ASSERT_ALIGNED(*slot, test_struct);
		(*slot)->val1 = i;
	}

	ASSERT_EQ(slab_get(slab), NULL);

	// exercise all blocks' values
	for (slot = s, i = 0; i < final_cap; i++, slot++) {
		ASSERT_EQ((size_t)(*slot)->val1, i);
		ASSERT_EQ(slab_put(slab, *slot), 0);
	}

	for (slot = s, i = 0; i < final_cap; i++, slot++) {
		*slot = slab_get(slab);
		ASSERT_NEQ(*slot, NULL);
		ASSERT_ALIGNED(*slot, test_struct);
	}

	ASSERT_EQ(slab_get(slab), NULL);

	slab_free(slab);
	free(s);

	return 0;
}

int test_slab_init_null_initializers()
{
	size_t size = 4;
	slab_t* b = slab_create(size, sizeof(test_struct));

	ASSERT_NEQ(b, NULL);
	ASSERT_NEQ(b->blocks, NULL);

	ASSERT_NEQ(&b->blocks[0][0], NULL);
	ASSERT_NEQ(&b->blocks[0][1], NULL);
	ASSERT_NEQ(&b->blocks[0][2], NULL);
	ASSERT_NEQ(&b->blocks[0][3], NULL);

	slab_slot_t* block = *b->blocks;

	ASSERT_EQ(b->next, block);
	ASSERT_EQ(b->next->next, ((void*)block) + b->slot_size * 1);
	ASSERT_EQ(b->next->next->next, ((void*)block) + b->slot_size * 2);
	ASSERT_EQ(b->next->next->next->next, ((void*)block) + b->slot_size * 3);
	ASSERT_EQ(b->next->next->next->next->next, NULL);

	slab_free(b);

	return 0;
}

int test_slab_aligned()
{
	ASSERT_ALIGNED_INIT(slab_t, slab_init, slab_deinit, slab_get, struct one)
	ASSERT_ALIGNED_INIT(slab_t, slab_init, slab_deinit, slab_get, struct two)
	ASSERT_ALIGNED_INIT(slab_t, slab_init, slab_deinit, slab_get, struct three)
	ASSERT_ALIGNED_INIT(slab_t, slab_init, slab_deinit, slab_get, struct four)
	ASSERT_ALIGNED_INIT(slab_t, slab_init, slab_deinit, slab_get, struct five)
	ASSERT_ALIGNED_INIT(slab_t, slab_init, slab_deinit, slab_get, struct six)
	ASSERT_ALIGNED_INIT(slab_t, slab_init, slab_deinit, slab_get, struct seven)

	return 0;
}

int main(int argc, char* argv[])
{
	test_ctx_t ctx;

	TEST_INIT(ctx, argc, argv);

	TEST_RUN(ctx, test_slab_create);
	TEST_RUN(ctx, test_slab_free_null);
	TEST_RUN(ctx, test_slab_get);
	TEST_RUN(ctx, test_slab_resize);
	TEST_RUN(ctx, test_slab_init_null_initializers);
	TEST_RUN(ctx, test_slab_aligned);

	TEST_RELEASE(ctx);
}
