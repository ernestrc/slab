#include "test.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../src/circ_array.h"
#include "test.h"

int test_circ_array_create()
{
	circ_array_t* a = circ_array_create(100);

	ASSERT_NEQ(a->a, NULL);
	circ_array_free(a);

	return 0;
}

int test_circ_array_free_null()
{
	circ_array_free(NULL);

	return 0;
}

int test_circ_array_free_circ_array_null()
{
	circ_array_t b;
	b.a = NULL;
	circ_array_free(&b);

	return 0;
}

int test_circ_array_get()
{
}

int main(int argc, char* argv[])
{
	test_ctx_t ctx;
	TEST_INIT(ctx, argc, argv);

	TEST_RUN(ctx, test_circ_array_create);
	TEST_RUN(ctx, test_circ_array_free_null);
	TEST_RUN(ctx, test_circ_array_free_circ_array_null);
	TEST_RUN(ctx, test_circ_array_get);

	TEST_RELEASE(ctx);
}
