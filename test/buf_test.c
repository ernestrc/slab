#include "test.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../src/buf.h"
#include "test.h"

int test_buf_create()
{
	buf_t* b = buf_create(100);

	ASSERT_NEQ(b->buf, NULL);
	buf_free(b);

	return 0;
}

int test_buf_free_null()
{
	buf_free(NULL);

	return 0;
}

int test_buf_free_buf_null()
{
	buf_t b;
	b.buf = NULL;
	buf_free(&b);

	return 0;
}

int test_buf_rw_1()
{
	int n;
	int cap;
	char data1[] = {1, 2, 3};
	char data2[3] = {};

	cap = 100;
	n = 3;

	buf_t* b = buf_create(cap);

	// write to buffer
	ASSERT_EQ(buf_write(b, data1, n), n);

	// write again, should overwrite previous
	// since extend is not called
	ASSERT_EQ(buf_write(b, data1, n), n);

	// before extend, no data available
	ASSERT_EQ(buf_read(b, data2, n), 0);
	ASSERT_EQ(buf_readable(b), 0);
	ASSERT_EQ(buf_writable(b), cap);

	buf_extend(b, n);

	ASSERT_EQ(buf_readable(b), n);
	ASSERT_EQ(buf_writable(b), cap - n);

	// read from buffer
	ASSERT_EQ(buf_read(b, data2, n), n);

	ASSERT_MEM_EQ(data1, data2, n);

	// consume
	buf_consume(b, n);

	ASSERT_EQ(buf_readable(b), 0);
	ASSERT_EQ(buf_writable(b), cap);

	// free
	buf_free(b);

	return 0;
}

int test_buf_rw_2()
{
	int n;
	int cap;
	char data1[] = {1, 2, 3};
	char data2[10] = {0};

	cap = 10;
	n = 3;

	buf_t* b = buf_create(cap);

	ASSERT_EQ(buf_write(b, data1, n), n);
	buf_extend(b, n);
	ASSERT_EQ(buf_write(b, data1, n), n);
	buf_extend(b, n);
	ASSERT_EQ(buf_write(b, data1, n), n);
	buf_extend(b, n);

	ASSERT_EQ(buf_readable(b), 9);
	ASSERT_EQ(buf_writable(b), cap - 9);

	ASSERT_EQ(buf_read(b, data2, cap), 9);

	char expected[] = {1, 2, 3, 1, 2, 3, 1, 2, 3};
	ASSERT_MEM_EQ(data2, expected, 9);

	// consume
	buf_consume(b, 9);

	ASSERT_EQ(buf_readable(b), 0);
	ASSERT_EQ(buf_writable(b), cap);

	// free
	buf_free(b);

	return 0;
}

int test_buf_overflow()
{
	int cap = 5;
	char data[] = {1, 1, 1, 1, 1, 1}; // 6

	buf_t* b = buf_create(cap);

	ASSERT_EQ(buf_write(b, data, 6), -1);
	ASSERT_EQ(buf_write(b, data, cap), cap);

	buf_free(b);

	return 0;
}

int test_buf_reserve()
{
	char data[] = {1, 1, 1};

	buf_t* b = buf_create(1);

	ASSERT_EQ(buf_writable(b), 1);
	ASSERT_EQ(buf_write(b, data, 3), -1);

	buf_reserve(b, 1);
	ASSERT_EQ(buf_writable(b), 2);

	ASSERT_EQ(buf_write(b, data, 3), -1);

	buf_reserve(b, 1);
	ASSERT_EQ(buf_writable(b), 3);

	ASSERT_EQ(buf_write(b, data, 3), 3);
	buf_extend(b, 3);

	char data2[] = {0, 0, 0};

	ASSERT_EQ(buf_read(b, data2, 3), 3);
	ASSERT_MEM_EQ(data, data2, 3);

	buf_free(b);

	return 0;
}

int test_buf_reset()
{
	buf_t* b = buf_create(1);
	ASSERT_EQ(buf_writable(b), 1);

	buf_reserve(b, 1);
	ASSERT_EQ(buf_writable(b), 2);

	buf_reset(b);
	ASSERT_EQ(buf_writable(b), 1);

	buf_free(b);

	return 0;
}

int test_buf_nonoverlap()
{
	int cap = 5;

	buf_t* b = buf_create(cap);

	errno = 0;
	ASSERT_EQ(buf_write(b, b->buf + 2, 1), -1);
	ASSERT_EQ(errno, EINVAL);

	buf_free(b);

	return 0;
}

int test_buf_compact()
{
	long n;
	int cap;
	char data1[] = {1, 2, 3};
	char data2[10] = {0};

	cap = 10;
	n = 3;

	buf_t* b = buf_create(cap);

	ASSERT_EQ(buf_write(b, data1, n), n);
	buf_extend(b, n);
	ASSERT_EQ(buf_write(b, data1, n), n);
	buf_extend(b, n);
	ASSERT_EQ(buf_write(b, data1, n), n);
	buf_extend(b, n);

	ASSERT_EQ(buf_readable(b), 9);
	ASSERT_EQ(buf_writable(b), cap - 9);

	ASSERT_EQ(buf_read(b, data2, cap), 9);

	char expected[] = {1, 2, 3};
	ASSERT_MEM_EQ(data2, expected, 3);
	buf_consume(b, 3);

	char data3[] = {9, 9, 9};

	// buffer has space for 1 more chars
	// but it should have space for 4
	ASSERT_EQ(buf_writable(b), 1);
	ASSERT_EQ(buf_readable(b), 6);
	ASSERT_EQ(buf_write(b, data3, 3), -1);

	ASSERT_EQ(buf_compact(b), 1);
	ASSERT_EQ(buf_writable(b), 4);
	ASSERT_EQ(buf_readable(b), 6);

	ASSERT_EQ(buf_write(b, data3, 3), 3);
	buf_extend(b, 3);

	ASSERT_EQ(buf_readable(b), 9);
	ASSERT_EQ(buf_writable(b), cap - 9);

	ASSERT_EQ(buf_read(b, data2, cap), 9);

	char expected2[] = {1, 2, 3, 1, 2, 3, 9, 9, 9};
	ASSERT_MEM_EQ(data2, expected2, 9);

	buf_consume(b, 9);

	ASSERT_EQ(buf_readable(b), 0);
	ASSERT_EQ(buf_writable(b), cap);

	buf_free(b);

	return 0;
}

int main(int argc, char* argv[])
{
	test_ctx_t ctx;
	TEST_INIT(ctx, argc, argv);

	TEST_RUN(ctx, test_buf_create);
	TEST_RUN(ctx, test_buf_free_null);
	TEST_RUN(ctx, test_buf_free_buf_null);
	TEST_RUN(ctx, test_buf_rw_1);
	TEST_RUN(ctx, test_buf_rw_2);
	TEST_RUN(ctx, test_buf_overflow);
	TEST_RUN(ctx, test_buf_reserve);
	TEST_RUN(ctx, test_buf_reset);
	TEST_RUN(ctx, test_buf_nonoverlap);
	TEST_RUN(ctx, test_buf_compact);

	TEST_RELEASE(ctx);
}
