#pragma once
#include <stddef.h>
struct obj {
    void *info;
    int cnt;
};

typedef struct obj obj_t;

/**
 */
void deallocate(obj_t *);

/**
 */
obj_t *allocate(size_t size);

/** 
*/
void retain(obj_t *obj);

/**
 */
void release(obj_t *obj);


obj_t *allocate_array(size_t elements, size_t elem_size);
