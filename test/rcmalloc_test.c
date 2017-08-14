#include "test.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../src/rcmalloc.h"

size_t BUF_CAP = 16;

int test_rcmalloc()
{
	char* buf = rcmalloc(sizeof(char));
	*buf = '1';
	rcfree(buf);
	return 0;
}

int test_rcmalloc_dup()
{
	char *buf, *buf2, *buf_dup;

	buf = rcmalloc(sizeof(char));
	ASSERT_NEQ(buf2 = rcmalloc(sizeof(char)), NULL);
	buf_dup = rcdup(buf);
	ASSERT_EQ(buf_dup, buf);

	rcfree(buf);
	ASSERT_EQ(rcmalloc(sizeof(char)), NULL);

	rcfree(buf_dup);
	ASSERT_NEQ(rcmalloc(sizeof(char)), NULL);

	rcfree(buf);
	return 0;
}

int main(int argc, char* argv[])
{
	test_ctx_t ctx;

	TEST_INIT(ctx, argc, argv);

	ASSERT_NEQ(rcmalloc_init(2), -1);

	TEST_RUN(ctx, test_rcmalloc);
	TEST_RUN(ctx, test_rcmalloc_dup);

	rcmalloc_deinit();

	TEST_RELEASE(ctx);
}
