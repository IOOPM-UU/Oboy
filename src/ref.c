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

obj_t *allocate(size_t size){
    obj_t *obj = calloc(1, size);
    obj->cnt = 1;
    return obj;
}

void retain(obj_t *obj) {
    obj->cnt++;
} 

void release(obj_t *obj) {
    obj->cnt--;
}

size_t rc(obj_t *obj) {
    return obj->cnt;
}

