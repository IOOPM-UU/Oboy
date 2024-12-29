#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "inlupp2_DONOTTOUCH/generic_data_structures/hash_table.h"
#include "inlupp2_DONOTTOUCH/generic_data_structures/linked_list.h"
#include "inlupp2_DONOTTOUCH/business_logic/common.h"

// Retrieves the metadata for the object, which is stored just before the object on the heap
#define GET_METADATA(obj) ((metadata_t *)((char *)(obj) - sizeof(metadata_t)))

/// Dummy 'obj' is just a void pointer as far as this manager is concerned
typedef void obj;

/// A destructor function pointer
typedef void(*function1_t)(obj *);

/// The memory header we store immediately before the actual data
struct metadata {
    size_t rc;                      ///< reference count
    function1_t destructor;        ///< function to call before freeing
    size_t size;                   ///< total size of the user allocation
};
typedef struct metadata metadata_t;

// The required functions
void retain(obj *);
void release(obj *);
size_t rc(obj *);
obj *allocate(size_t bytes, function1_t destructor);
obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor);
metadata_t *memdata_generate(function1_t destructor, size_t size);
void deallocate(obj *);
void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();
ioopm_list_t *get_schedule_linked_list();
void free_scheduled_tasks(size_t size);
void add_to_schedule(obj *object);
void free_all();