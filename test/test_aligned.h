#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

struct one {
	char a;
};

struct two {
	short a;
};

struct three {
	int a;
};

struct four {
	int* a;
};

struct five {
	int* a;
	int* b;
	int* c;
	int* d;
	int* e;
};

struct six {
	struct five a;
	struct five b;
	struct five c;
	struct five d;
	struct five e;
};

struct seven {
	short e;
} __attribute__((aligned(alignof(max_align_t))));

#define ASSERT_ALIGNED(ptr, o)                                                 \
	ASSERT_FALSE((((uintptr_t)(ptr)) & ((uintptr_t)(alignof(o) - 1))));

#define ASSERT_ALIGNED_INIT(ss_t, ss_t_init, ss_t_deinit, ss_t_get, o)         \
	{                                                                          \
		ss_t s;                                                                \
		ss_t_init(&s, 1, sizeof(o));                                           \
		ASSERT_ALIGNED(ss_t_get(&s), o);                                       \
		ss_t_deinit(&s);                                                       \
	}
