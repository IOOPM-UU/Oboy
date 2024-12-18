#pragma once
#include <stdlib.h>
#include <stddef.h>
#include "inlupp2/generic_data_structures/hash_table.h"
#include "inlupp2/generic_data_structures/linked_list.h"
#include "inlupp2/business_logic/common.h"

struct memdata{
    size_t rc;
    function1_t destructor;
};
typedef struct memdata memdata_t;

typedef void obj;
typedef void(*function1_t)(obj *);

void retain(obj *);
void release(obj *);
size_t rc(obj *);

obj *allocate(size_t bytes, function1_t destructor);
obj*allocate_array(size_t elements, size_t elem_size, function1_t destructor);

void deallocate(obj *);
void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();