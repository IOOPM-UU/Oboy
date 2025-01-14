#include "refmem.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

static lib_list_t *schedule_linked_list = NULL;
static lib_hash_table_t *metadata_ht = NULL;
static bool release_in_progress = false; 
static const size_t STANDARD_CASCADE_LIMIT = 100;
static size_t CASCADE_LIMIT = 100;

bool int_equal(lib_elem_t a, lib_elem_t b) {
    return a.i == b.i;
}
 
static int default_hash_function(lib_elem_t value) {
    return value.i;
} 

static void initialize_collector() {
    if (!metadata_ht) {
        metadata_ht = lib_hash_table_create(int_equal, NULL, default_hash_function);
    }
    if (!schedule_linked_list) {
        schedule_linked_list = lib_linked_list_create(int_equal); // TODO!!! int_equal, but ptr_elem values
    }
}

lib_hash_table_t *get_metadata_ht() {
    return metadata_ht;
}

lib_list_t *get_schedule_linked_list() {
    return schedule_linked_list;
}

metadata_t *metadata_generate(function1_t destructor, size_t size) {
    metadata_t *metadata = calloc(1, sizeof(metadata_t));
    if (!metadata) return NULL;
    metadata->rc         = 0;
    metadata->destructor = destructor;
    metadata->size       = size;
    return metadata;
}

static metadata_t *get_metadata(obj* object) {
    uintptr_t key_as_int = (uintptr_t) object;
    lib_option_t found_data = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));
    if(!found_data.success) return NULL;
    return (metadata_t*) found_data.value.p;
}

uint8_t rc(obj* object) {
    metadata_t *metadata = get_metadata(object);
    if (metadata) {
        return metadata->rc;
    } else {
        printf("metadata could not be found!");
        return 0;
    }
}

static bool is_valid_pointer(void *object){
    if (!object) return false;
    uintptr_t key_as_int = (uintptr_t) object;
    lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));
    return option.success;
}

static void default_destructor(obj* object) {
    if(!object) return;
    
    metadata_t *metadata = get_metadata(object);
    if(!metadata) return;
    
    size_t object_size = metadata->size;
    destructor_loop(object_size, object);
}

static void destructor_loop(size_t object_size, obj *object) {
    for (size_t offset = 0; offset + sizeof(void *) <= object_size; offset++)
    {
        void **possible_pointer = (void **)((char *)object + offset);
        if (is_valid_pointer(*possible_pointer))
        {
            release(*possible_pointer);
        }
    }
}

static void add_to_schedule(obj *object) {
    metadata_t *metadata = get_metadata(object);
    if(!metadata) return;
    lib_linked_list_append(get_schedule_linked_list(), lib_ptr_elem(object));
    if (metadata->destructor) { 
        metadata->destructor(object);
    }
    else {
        default_destructor(object);
    }
}

static void task_manager_loop(size_t size) {
    size_t freed_size = 0;
    size_t cascade_amount = 0;
    while ((freed_size < size || cascade_amount < CASCADE_LIMIT) &&
           lib_linked_list_size(get_schedule_linked_list()) > 0)
    {
        bool successful1 = false;
        obj *to_remove = lib_linked_list_get(get_schedule_linked_list(), 0, &successful1).p;
        deallocate(to_remove);
        cascade_amount++;
    }
    release_in_progress = false;
}

static void schedule_task_manager(obj *object, size_t size) {
    release_in_progress = true;
    metadata_t *metadata = get_metadata(object);
    if(object && metadata) {
        if(metadata->rc == 0) {
            add_to_schedule(object);
        }
    }
    task_manager_loop(size);
}

obj *allocate(size_t bytes, function1_t destructor) {
    initialize_collector();
    schedule_task_manager(NULL, bytes);
    obj *object = calloc(1, bytes);
    uintptr_t key_as_int = (uintptr_t) object;
    metadata_t *metadata = metadata_generate(destructor, bytes);
    lib_hash_table_insert(get_metadata_ht(), lib_int_elem(key_as_int), lib_ptr_elem(metadata));
    return object;
}

obj *allocate_array(size_t elements, size_t bytes, function1_t destructor) {
    size_t total_size = elements * bytes;
    return allocate(total_size, destructor);
}

void deallocate(obj* object) {
    if(!object) return; 
    uintptr_t key_as_int = (uintptr_t)object;
    metadata_t *metadata = get_metadata(object);
    if(!metadata) return;
    assert(rc(object) == 0);
    lib_list_option_t exist = lib_linked_list_contains(schedule_linked_list, lib_ptr_elem(object));
    if(exist.success) {
        bool successful1 = false;
        lib_linked_list_remove(schedule_linked_list, exist.value.i, &successful1);
    }
    lib_hash_table_remove(get_metadata_ht(), lib_int_elem(key_as_int));
    free(metadata);
    free(object);
}

void retain(obj *object) {
    if (!object) return;
    uintptr_t key_as_int = (uintptr_t)object;
    lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));
    if (option.success) {
        metadata_t *metadata = (metadata_t *)(option.value.p);
        metadata->rc++;
    }
}

void release(obj *object) {
    if (!object) return;
    metadata_t *metadata = get_metadata(object);
    if (!metadata) return;
    else if(metadata->rc > 0) {
        metadata->rc--; 
    }
    if(metadata->rc == 0) {
        if(!release_in_progress){
            schedule_task_manager(object, 0);
        }
        else {
            add_to_schedule(object);   
        }
    }
}

void cleanup() {
    schedule_task_manager(NULL, SIZE_MAX); 
}

static void free_all() {
    cleanup();
    lib_linked_list_destroy(get_schedule_linked_list());
    schedule_linked_list = NULL;
    lib_hash_table_destroy(get_metadata_ht());
}

void shutdown() {
    set_cascade_limit(STANDARD_CASCADE_LIMIT);
    free_all();
}

size_t get_cascade_limit() {
    return CASCADE_LIMIT;
}

void set_cascade_limit(size_t size) {
    CASCADE_LIMIT = size; 
}

void str_dummy_destructor(obj *o) {
}

char *rc_strdup(char *src) {
    char *str;
    char *p;
    int len = 0;
    while (src[len]) len++;
    str = allocate(len + 1, str_dummy_destructor);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}

