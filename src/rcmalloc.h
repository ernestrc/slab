#ifndef LIBSLAB_MALLOC_H
#define LIBSLAB_MALLOC_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* initialize rcmalloc context */
int rcmalloc_init(size_t cap);

/* allocate size */
void* rcmalloc(size_t size);

/* duplicate rcmalloc'd ptr,
 * effectively incrementing ref counter */
void* rcdup(void* ptr);

/* free rcmalloc'd ptr  */
void rcfree(void* ptr);

void rcmalloc_deinit();

#ifdef __cplusplus
}
#endif
#endif
