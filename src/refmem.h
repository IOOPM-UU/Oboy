#pragma once
#include <stdlib.h>
#include <stddef.h>


typedef void obj;
typedef void(*function1_t)(obj *);

void retain(obj *);
void release(obj *);
size_t rc(obj *);
obj_t *allocate(size_t bytes, function1_t destructor);
obj_t*allocate_array(size_t elements, size_t elem_size, function1_t destructor);
void deallocate(obj *);
void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();