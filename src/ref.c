#include "ref.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

static ioopm_list_t *schedule_linked_list = NULL;

bool int_eq(elem_t a, elem_t b) {
    return a.i == b.i;
}

 
static int default_hash_function(elem_t value)
{
    return value.i;
} 

size_t CASCADE_LIMIT = 10;
 

ioopm_hash_table_t *get_metadata_ht() {
    static ioopm_hash_table_t *metadata_ht = NULL;
    if(metadata_ht == NULL){
        metadata_ht = ioopm_hash_table_create(int_eq, NULL, default_hash_function);
    }
    return metadata_ht;
}


ioopm_list_t *get_schedule_linked_list() 
{
    if (!schedule_linked_list) {
        schedule_linked_list = ioopm_linked_list_create(int_eq); 
    }
    return schedule_linked_list;
}

 
metadata_t *metadata_generate(function1_t destructor, size_t size) 
 {
     metadata_t *metadata = calloc(1, sizeof(metadata_t));
     if (!metadata) return NULL;

     metadata->rc         = 0;
     metadata->destructor = destructor;
     metadata->size       = size;
     return metadata;
 
}

void add_to_schedule(obj *object) 
{
    ioopm_linked_list_append(get_schedule_linked_list(), ptr_elem(object));
}

size_t rc(obj* object) {
    uintptr_t key_as_int = (uintptr_t)object;
    ioopm_option_t option = ioopm_hash_table_lookup(get_metadata_ht(), int_elem(key_as_int));
    if (Successful(option)) {
        metadata_t *metadata = (metadata_t *)(option.value.p);
        return metadata->rc;
    } else {
        printf("object could not be found!");
        return 0; // TODO should just let it crash instead???
    }
}

void release_destructor(obj *to_remove) 
{
    if (!to_remove) {
        return;
    }
    
    uintptr_t key_as_int = (uintptr_t)to_remove;
    
    
    ioopm_option_t option = ioopm_hash_table_lookup(get_metadata_ht(), int_elem(key_as_int));
    if (option.success) {
        metadata_t *metadata = (metadata_t *)(option.value.p);
        metadata->destructor(to_remove);
    }
}

static bool is_valid_pointer(void *object)
{
    if (!object) return false;
    //memdata_t *metadata = GET_METADATA(object);
    // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t)object;
    
    //memdata_t *metadata = GET_METADATA(object);
    ioopm_option_t option = ioopm_hash_table_lookup(get_metadata_ht(), int_elem(key_as_int));

    if (option.success) return true;
     
    return false;
}

void default_destructor(obj* object)
{
    if(!object)
    {
        return;
    }
    //memdata_t *metadata = GET_METADATA(object);
    uintptr_t key_as_int = (uintptr_t)object;
    
    //memdata_t *metadata = GET_METADATA(object);
    ioopm_option_t option = ioopm_hash_table_lookup(get_metadata_ht(),int_elem(key_as_int));

    if (!option.success) return;
     metadata_t *metadata = (metadata_t *)(option.value.p);
    size_t object_size = metadata->size;

    for(size_t offset = 0; offset <= object_size - sizeof(void*); offset++)
    {
        void **possible_pointer = (void **)((char *)object + offset);
        if(is_valid_pointer(*possible_pointer))
        {
            release(*possible_pointer);
            offset += sizeof(void*) - 1; // -1 since for loop increments
        }
    }
}

void free_scheduled_tasks(size_t size) 
{
    size_t freed_size = 0;
    size_t freed_amount = 0;

    while ((freed_size < size && freed_amount < CASCADE_LIMIT) && 
        ioopm_linked_list_size(get_schedule_linked_list()) > 0) 
    {
        bool successful1 = false;
        obj *to_remove = ioopm_linked_list_get(get_schedule_linked_list(), 0, &successful1).p;

        if (!successful1 || !to_remove) 
        {
            printf("Null or invalid object encountered during task freeing\n");
            break; 
        }

       // memdata_t *metadata = GET_METADATA(to_remove);
       // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t)to_remove;
    
    //memdata_t *metadata = GET_METADATA(object);
    
    // Lookup metadata in the hash table
    ioopm_option_t option = ioopm_hash_table_lookup(get_metadata_ht(), int_elem(key_as_int));
        if (!option.success)
        {
            // If no metadata is found, remove the pointer from the schedule
            bool success_remove = false;
            ioopm_linked_list_remove(get_schedule_linked_list(), 0, &success_remove);
            if (!success_remove) 
            {
                printf("Failed to remove invalid object from list.\n");
            }
            continue;
        }
        metadata_t *metadata = (metadata_t *)(option.value.p);       
        // If reference count is nonzero, skip freeing
        if (metadata->rc > 0) 
        {
            printf("Skipping object with active references (rc=%zu).\n", metadata->rc);
            break;
        }

        // Check if adding this objects size would exceed `size`:
        if (freed_size + metadata->size > size)
        {
            // We would exceed the free size limit
            break;
        }
        freed_size += metadata->size;
        
        // if (metadata->destructor) // This if else could maybe be extracted to deallocate (probably including the free after, and maybe even free(metadata))
        // {
        //     metadata->destructor(to_remove);
        // }
        // else
        // {
        //     default_destructor(to_remove);
        // }
        free(to_remove);

        // Remove from the scheduled list
        bool successful2 = false;
        ioopm_linked_list_remove(get_schedule_linked_list(), 0, &successful2);
        if (!successful2) 
        {
            printf("Failed to remove object from linked list\n");
            break;
        }
        // Remove from the metadata hash table
        ioopm_hash_table_remove(get_metadata_ht(), int_elem(key_as_int));
        // Finally free the metadata
        free(metadata);
        freed_amount++;
    }
}

obj *allocate(size_t bytes, function1_t destructor) 
{
    free_scheduled_tasks(bytes); 
    obj *object = malloc(bytes);
    if (!object) 
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t)object;

    metadata_t *metadata = metadata_generate(destructor, bytes);
    if (!metadata) 
    {
        printf("Failed to allocate memory for metadata + object\n");
        return NULL;
    }

    //memdata_t *metadata = calloc(1, sizeof(memdata_t) + bytes);
    ioopm_hash_table_insert(get_metadata_ht(), int_elem(key_as_int), ptr_elem(metadata));
     

     
    return object;
}

// Allocate array
obj *allocate_array(size_t elements, size_t bytes, function1_t destructor) 
{
    size_t total_size = elements * bytes;
    return allocate(total_size, destructor);
}

// Increase reference count
void retain(obj *object) 
{
    if (!object) return;

    // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t)object;
    
    //memdata_t *metadata = GET_METADATA(object);
    ioopm_option_t option = ioopm_hash_table_lookup(get_metadata_ht(),int_elem(key_as_int));

    if (option.success) {
        metadata_t *metadata = (metadata_t *)(option.value.p);
        metadata->rc++;
    }
}

void release(obj *object) 
{
    if (!object) return;

    //memdata_t *metadata = GET_METADATA(object);
     // Convert pointer to integer using uintptr_t
    uintptr_t key_as_int = (uintptr_t)object;
    
    //memdata_t *metadata = GET_METADATA(object);
    ioopm_option_t option = ioopm_hash_table_lookup(get_metadata_ht(),int_elem(key_as_int));

    if (!option.success) return;
     metadata_t *metadata = (metadata_t *)(option.value.p);  

    if (metadata->rc == 0) 
    {
        add_to_schedule(object);
        //TODO: flyttat in destructorer in hit (test)
        if (metadata->destructor) // This if else could maybe be extracted to deallocate (probably including the free after, and maybe even free(metadata))
        {
            metadata->destructor(object);
        }
        else
        {
            default_destructor(object);
    }
    } 
    else
    {
        metadata->rc--;
    }
    free_scheduled_tasks(0); // Doesnt need a size since it just works with cascade limit
}

void cleanup() 
{
     uintptr_t key_as_int;
    free_scheduled_tasks(SIZE_MAX); 

    while (ioopm_linked_list_size(get_schedule_linked_list()) > 0) 
    {
        bool successful = false;
        obj *object = ioopm_linked_list_get(get_schedule_linked_list(), 0, &successful).p;

        if (successful && object) 
        {
             key_as_int = (uintptr_t)object;
             ioopm_option_t option = ioopm_hash_table_lookup(get_metadata_ht(),int_elem(key_as_int));

        if (option.success){
            metadata_t *metadata = (metadata_t *)(option.value.p);  
            if (metadata->rc == 0) 
            {
                release_destructor(object);
                ioopm_hash_table_remove(get_metadata_ht(), int_elem(key_as_int));
            }
        }
             
        }
        ioopm_linked_list_remove(get_schedule_linked_list(), 0, &successful);
    }
}

void free_all() 
{
    cleanup();
    ioopm_linked_list_destroy(get_schedule_linked_list());
    schedule_linked_list = NULL;
    ioopm_hash_table_destroy(get_metadata_ht());
    
}

void shutdown() 
{
    free_all();
}

size_t get_cascade_limit() 
{
    return CASCADE_LIMIT;
}

void set_cascade_limit(size_t size) 
{
    CASCADE_LIMIT = (int)size; 
}
