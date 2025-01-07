#include "ref.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
static lib_list_t *schedule_linked_list = NULL;
static lib_hash_table_t *metadata_ht = NULL;

bool int_equal(lib_elem_t a, lib_elem_t b) {
    return a.i == b.i;
}

 
static int default_hash_function(lib_elem_t value){
    return value.i;
} 

size_t CASCADE_LIMIT = 10;
 
void initialize_collector() {
    if(!metadata_ht){
        metadata_ht = lib_hash_table_create(int_equal, NULL, default_hash_function);
    }
    if(!schedule_linked_list) {
        schedule_linked_list = lib_linked_list_create(int_equal); // TODO!!! int_equal, but ptr_elem values
    }
}

lib_hash_table_t *get_metadata_ht() {
    if(metadata_ht == NULL){
        metadata_ht = lib_hash_table_create(int_equal, NULL, default_hash_function);
    }
    return metadata_ht;
}


lib_list_t *get_schedule_linked_list() {
    if (!schedule_linked_list) {
        schedule_linked_list = lib_linked_list_create(int_equal); // TODO!!! int_equal, but ptr_elem values
    }
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

static metadata_t *get_metadata(obj* object){
    uintptr_t key_as_int = (uintptr_t)object;
    lib_option_t found_data = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));

    if(!found_data.success) return NULL;
    return (metadata_t*) found_data.value.p;
}

void add_to_schedule(obj *object) {
    lib_linked_list_append(get_schedule_linked_list(), lib_ptr_elem(object));
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

void release_destructor(obj *to_remove) {
    if (!to_remove) {
        return;
    }
    
    metadata_t *metadata = get_metadata(to_remove);
    if (metadata) {
        metadata->destructor(to_remove);
    }
}

static bool is_valid_pointer(void *object){
    if (!object) return false;
    //memdata_t *metadata = GET_METADATA(object);
    // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t) object;
    
    //memdata_t *metadata = GET_METADATA(object);
    lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));
    return option.success;
}

void default_destructor(obj* object){
    if(!object){
        return;
    }

    uintptr_t key_as_int = (uintptr_t) object;
    
    lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));

    if (!option.success) return;
    metadata_t *metadata = (metadata_t *)(option.value.p);
    size_t object_size = metadata->size;

    for (size_t offset = 0; offset + sizeof(void *) <= object_size; offset += sizeof(void *))
    {
        void **possible_pointer = (void **)((char *)object + offset);
        if (is_valid_pointer(*possible_pointer))
        {
            release(*possible_pointer);
        }
    }
}

void free_scheduled_tasks(size_t size){
    
    size_t freed_size = 0;
    size_t freed_amount = 0;
    if(!schedule_linked_list || !metadata_ht) return; //are they initialized?

    while ((freed_size < size || freed_amount < CASCADE_LIMIT) && 
        lib_linked_list_size(get_schedule_linked_list()) > 0)
    {
        bool successful1 = false;
        obj *to_remove = lib_linked_list_get(get_schedule_linked_list(), 0, &successful1).p;  // TODO!!! int_equal, but ptr_elem values

        if (!successful1 || !to_remove){
            printf("Null or invalid object encountered during task freeing\n");
            break; 
        }

        // memdata_t *metadata = GET_METADATA(to_remove);
        // Convert pointer to integer using uintptr_t
        uintptr_t key_as_int = (uintptr_t)to_remove;  // TODO!!! int_equal, but ptr_elem values
        
        //memdata_t *metadata = GET_METADATA(object);
        
        // Lookup metadata in the hash table
        // lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), int_elem(key_as_int));
        metadata_t *metadata = get_metadata(to_remove);
        if (!metadata){
            // If no metadata is found, remove the pointer from the schedule
            bool success_remove = false;
            lib_linked_list_remove(get_schedule_linked_list(), 0, &success_remove);
            if (!success_remove){
                printf("Failed to remove invalid object from list.\n");
            }
            continue;
        }
        // metadata_t *metadata = (metadata_t *)(option.value.p);       
        // If reference count is nonzero, skip freeing
        if (metadata->rc > 0) {
            printf("Skipping object with active references (rc=%u).\n", metadata->rc);
            bool success_remove = false;
        }

        freed_size += metadata->size;
        
        //TODO: destructor if-cases were here originally /david

        free(to_remove);

        // Remove from the scheduled list
        bool successful2 = false;
        lib_linked_list_remove(get_schedule_linked_list(), 0, &successful2);
        if (!successful2) {
            printf("Failed to remove object from linked list\n");
            break;
        }
        // Remove from the metadata hash table
        lib_hash_table_remove(get_metadata_ht(), lib_int_elem(key_as_int));
        // Finally free the metadata
        free(metadata);
        freed_amount++;
    }
}

obj *allocate(size_t bytes, function1_t destructor) {
    //initialize refcount 
    //free_scheduled_tasks(bytes); 

    obj *object = calloc(1, bytes);
    // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t)object;
    metadata_t *metadata = metadata_generate(destructor, bytes);

    lib_hash_table_insert(get_metadata_ht(), lib_int_elem(key_as_int), lib_ptr_elem(metadata));
    return object;
}

// Allocate array
obj *allocate_array(size_t elements, size_t bytes, function1_t destructor) {
    size_t total_size = elements * bytes;
    return allocate(total_size, destructor);
}

void deallocate(obj* object){
    uintptr_t key_as_int = (uintptr_t)object;
    metadata_t *metadata = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int)).value.p;
    add_to_schedule(object);
    //TODO: flyttat in destructorer in hit (david)
    if (metadata->destructor){ // This if else could maybe be extracted to deallocate (probably including the free after, and maybe even free(metadata))
        metadata->destructor(object);
    }
    else{
        default_destructor(object);
    }
    //free_scheduled_tasks(0); // Doesnt need a size since it just works with cascade limit
}

// Increase reference count
void retain(obj *object) {
    if (!object) return;

    // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t)object;
    
    //memdata_t *metadata = GET_METADATA(object);
    lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));

    if (option.success) {
        metadata_t *metadata = (metadata_t *)(option.value.p);
        metadata->rc++;
    }
}

void release(obj *object) {
    
    if (!object) return;

    // memdata_t *metadata = GET_METADATA(object);
    // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t)object;
    
    //memdata_t *metadata = GET_METADATA(object);
    lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));

    if (!option.success) return;
    metadata_t *metadata = (metadata_t *)(option.value.p);  

    if (metadata->rc > 0) {
        metadata->rc--; 
    }
    
    if(metadata->rc == 0) {
      deallocate(object);
    }
}

void cleanup() {
    uintptr_t key_as_int;
    free_scheduled_tasks(SIZE_MAX); 

    while (lib_linked_list_size(get_schedule_linked_list()) > 0) {
        bool successful = false;
        obj *object = lib_linked_list_get(get_schedule_linked_list(), 0, &successful).p;

        if (successful && object) {
            key_as_int = (uintptr_t)object;
            lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));

            if (option.success){
                metadata_t *metadata = (metadata_t *)(option.value.p);  
                if (metadata->rc == 0){
                    release_destructor(object);
                    lib_hash_table_remove(get_metadata_ht(), lib_int_elem(key_as_int));
                }
            }
             
        }
        lib_linked_list_remove(get_schedule_linked_list(), 0, &successful);
    }
}

void free_all() {
    cleanup();
    lib_linked_list_destroy(get_schedule_linked_list());
    schedule_linked_list = NULL;
    lib_hash_table_destroy(get_metadata_ht());
}

void shutdown() {
    free_all();
}

size_t get_cascade_limit() {
    return CASCADE_LIMIT;
}

void set_cascade_limit(size_t size) {
    CASCADE_LIMIT = (int)size; 
}

static void string_dummy_destructor(obj *o) {
    // No pointer scanning, do nothing
}

char *rc_strdup(char *src)
{
    char *str;
    char *p;
    int len = 0;

    while (src[len]) len++;

    str = allocate(len + 1, string_dummy_destructor);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}

