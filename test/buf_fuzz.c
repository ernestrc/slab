#include <stdint.h>
#include <stdio.h>

#include "../src/buf.h"

// assumes size <= 4096 which is the default for the fuzzer
#define MAX_SIZE 4096
#define INITIAL_SIZE 2

int LLVMFuzzerTestOneInput(const uint8_t data, size_t size)
{
	static buf_t b1;
	static buf_t b2;
	int n;

	if (b1.cap == 0) {
		buf_init(&b1, INITIAL_SIZE);
		buf_init(&b2, MAX_SIZE);
	}

	while (buf_writable(&b1) < size) {
		buf_compact(&b1);
		// duplicate cap
		if ((buf_reserve(&b1, b1.cap)) == -1)
			return 0;
		// move data to b2
		if ((n = buf_read(&b1, b2.next_write, size)) == -1)
			return 0;
		buf_consume(&b1, n);
	}

	n = buf_write(&b1, (const char*)&data, size);
	buf_extend(&b1, n);

	if (buf_readable(&b1) < b1.cap / 2)
		buf_reset(&b1);

	return 0;
}
