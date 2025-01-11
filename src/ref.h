#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "lib/lib_hash_table.h"
#include "lib/lib_linked_list.h"
#include "lib/lib_common.h"

// #include "lib/hash_table.h"
// #include "lib/linked_list.h"
// #include "lib/common.h"


// Retrieves the metadata for the object, which is stored just before the object on the heap
// #define GET_METADATA(obj) ((metadata_t *)((char *)(obj) - sizeof(metadata_t)))

/// Dummy 'obj' is just a void pointer as far as this manager is concerned
typedef void obj;

/// A destructor function pointer
typedef void(*function1_t)(obj *);

/// The memory header we store immediately before the actual data
struct metadata {
    uint8_t rc;                      ///< reference count //TODO
    function1_t destructor;        ///< function to call before freeing
    size_t size;                   ///< total size of the user allocation
};
typedef struct metadata metadata_t;

// The required functions
void retain(obj *);
void release(obj *);
uint8_t rc(obj *);
obj *allocate(size_t bytes, function1_t destructor);
void destructor_loop(size_t object_size, obj *object);
void task_manager_loop(size_t size);
obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor);
metadata_t *metadata_generate(function1_t destructor, size_t size);
void deallocate(obj *);
void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();

lib_hash_table_t *get_metadata_ht();
lib_list_t *get_schedule_linked_list();
char *rc_strdup(char *src);

// does nothing
void str_dummy_destructor(obj *o);
