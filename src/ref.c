#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ref.h"


void deallocate(obj_t *obj) {
    if (obj->cnt == 1) {
        free(obj);
        return;
    }
}

obj_t *allocate(size_t size, function1_t destructor){
    obj_t *obj = calloc(1, size);
    obj->cnt = 1;
    return obj;
}

void retain(obj_t *obj) {
    obj->cnt++;
} 

void release(obj_t *obj) {
    obj->cnt--;
    if (obj->cnt == 1) {
        deallocate(obj);
    }
}

obj_t *allocate_array(size_t elements, size_t elem_size, function1_t destructor) {
        obj_t *obj = calloc(elements, elem_size);
        obj->cnt = 1;
        return obj;
}

size_t rc(obj_t *obj) {
    return obj->cnt;
}

