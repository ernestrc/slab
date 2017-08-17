#include <stdint.h>

#include "../src/buf.h"

int LLVMFuzzerTestOneInput(const uint8_t data, size_t size) {
	static buf_t* b;
	if (!b) {
		b = buf_create(1024);

	}
	buf_write(b, (const char *) &data, size);

	return 0;
}
