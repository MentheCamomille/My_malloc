#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <stddef.h>

void initialize_heap();
void* my_malloc(size_t size);
void* my_malloc_best_fit(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t new_size);

#endif
