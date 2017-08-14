#include "test.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../src/rc_slab.h"

size_t BUF_CAP = 16;

int test_rc_slab_free_null()
{
	rc_slab_free(NULL);
	return 0;
}

int test_rc_slab_create()
{
	size_t size = 4;
	rc_slab_t* b = rc_slab_create(size, sizeof(char));

	ASSERT_NEQ(b, NULL);
	ASSERT_NEQ(b->slab.next, NULL);

	rc_slab_free(b);

	return 0;
}

int test_rc_slab_get()
{
	size_t size = 512;
	rc_slab_t* b = rc_slab_create(size, sizeof(char));

	char* value;
	size_t i;

	for (i = 0; i < size; ++i) {
		value = rc_slab_get(b);
		ASSERT_NEQ(value, NULL);
	}

	ASSERT_NULL(rc_slab_get(b));
	rc_slab_free(b);

	return 0;
}

int test_rc_slab_put()
{
	size_t size = 2;
	char* slot[2];

	rc_slab_t* s = rc_slab_create(size, sizeof(char));

	slot[0] = rc_slab_get(s);
	slot[1] = rc_slab_get(s);
	ASSERT_NULL(rc_slab_get(s));

	rc_slab_put(s, slot[0]);
	ASSERT_NEQ(slot[0] = rc_slab_get(s), NULL);
	ASSERT_NEQ(slot[0], slot[1]);

	rc_slab_free(s);

	return 0;
}

int test_rc_slab_dup()
{
	size_t size = 2;
	char* slot[2];

	rc_slab_t* s = rc_slab_create(size, sizeof(char));

	slot[0] = rc_slab_get(s);

	rc_slab_dup(slot[0]);
	rc_slab_put(s, slot[0]);

	slot[1] = rc_slab_get(s);
	ASSERT_EQ(rc_slab_get(s), NULL);

	rc_slab_put(s, slot[0]);
	ASSERT_NEQ(slot[0] = rc_slab_get(s), NULL);
	ASSERT_NEQ(slot[0], slot[1]);

	rc_slab_free(s);

	return 0;
}

int main(int argc, char* argv[])
{
	test_ctx_t ctx;
	TEST_INIT(ctx, argc, argv);

	TEST_RUN(ctx, test_rc_slab_create);
	TEST_RUN(ctx, test_rc_slab_free_null);
	TEST_RUN(ctx, test_rc_slab_get);
	TEST_RUN(ctx, test_rc_slab_put);
	TEST_RUN(ctx, test_rc_slab_dup);

	TEST_RELEASE(ctx);
}
