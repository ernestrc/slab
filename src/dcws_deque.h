#ifndef LIBSLAB_DCWS_DEQUEUE_H
#define LIBSLAB_DCWS_DEQUEUE_H

typedef struct dcws_deque_s {
	slab_t* slab;
} dcws_deque_t;

dcws_deque_t* dcws_deque_create(size_t cap, size_t slot_size);

void dcws_deque_init(dcws_deque_t* q, size_t cap, size_t slot_size);

void dcws_deque_push_bottom(dcws_deque_t* q, void* o);

void* dcws_deque_pop_bottom(dcws_deque_t* q);

void* dcws_deque_steal(dcws_deque_t* q);

#endif
