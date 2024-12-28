#include "ref.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

static ioopm_list_t *schedule_linked_list = NULL;

bool int_eq(void *a, void *b) {
    return *(int *)a == *(int *)b;
}

int CASCADE_LIMIT = 10;


ioopm_list_t *get_schedule_linked_list() 
{
    if (!schedule_linked_list) {
        schedule_linked_list = ioopm_linked_list_create(int_eq); 
    }
    return schedule_linked_list;
}

// memdata_t *memdata_generate(function1_t destructor, size_t size) 
// {
//     memdata_t *memdata = calloc(1, sizeof(memdata_t));
//     if (!memdata) return NULL;

//     memdata->rc         = 0;
//     memdata->destructor = destructor;
//     memdata->size       = size;
//     return memdata;
// }

void add_to_schedule(obj *object) 
{
    ioopm_linked_list_append(get_schedule_linked_list(), ptr_elem(object));
}

void release_destructor(obj *to_remove) 
{
    if (!to_remove) {
        return;
    }
    memdata_t *metadata = GET_METADATA(to_remove);
    if (metadata && metadata->destructor) {
        metadata->destructor(to_remove);
    }
}

// Calling free_scheduled_tasks(INT_MAX) frees everything
void free_scheduled_tasks(size_t size) 
{
    size_t freed_size = 0;

    while (freed_size < size && ioopm_linked_list_size(get_schedule_linked_list()) > 0)
    {
        bool successful1 = false;
        obj *to_remove = ioopm_linked_list_get(get_schedule_linked_list(), 0, &successful1).p;

        if (!successful1 || !to_remove) 
        {
            printf("Null or invalid object encountered during task freeing\n");
            break; 
        }

        memdata_t *metadata = GET_METADATA(to_remove);
        if (!metadata)
        {
            bool success_remove = false;
            ioopm_linked_list_remove(get_schedule_linked_list(), 0, &success_remove);
            if (!success_remove) 
            {
                printf("Failed to remove invalid object from list.\n");
            }
            continue;
        }
       
        if (metadata->rc > 0) 
        {
            printf("Skipping object with active references (rc=%zu).\n", metadata->rc);
            break;
        }

        freed_size += metadata->size;
        
        if (metadata->destructor) 
        {
            metadata->destructor(to_remove);
        }

        bool successful2 = false;
        ioopm_linked_list_remove(get_schedule_linked_list(), 0, &successful2);
        if (!successful2) 
        {
            printf("Failed to remove object from linked list\n");
            break;
        }

        free(metadata);
    }
}

obj *allocate(size_t bytes, function1_t destructor) 
{
    free_scheduled_tasks(bytes); 

    memdata_t *metadata = calloc(1, sizeof(memdata_t) + bytes);
    if (!metadata) 
    {
        printf("Failed to allocate memory for metadata + object\n");
        return NULL;
    }

    metadata->rc = 0;
    metadata->destructor = destructor;
    metadata->size = bytes;

    obj *object = (obj *)(metadata + 1); 
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
    memdata_t *metadata = GET_METADATA(object);
    if (metadata) {
        metadata->rc++;
    }
}

void release(obj *object) 
{
    if (!object) return;

    memdata_t *metadata = GET_METADATA(object);
    if (!metadata) return;  

    if (metadata->rc == 0) 
    {
        add_to_schedule(object);
    } 
    else 
    {
        metadata->rc--;
    }
    free_scheduled_tasks(0);
}

static bool is_valid_pointer(void *object)
{
    if (!object) return false;
    memdata_t *metadata = GET_METADATA(object);
    if (metadata) {
        return true;
    }
    return false;
}

void default_destructor(obj* object)
{
    if(!object)
    {
        return;
    }
    memdata_t *metadata = GET_METADATA(object);
    if (!metadata)
    {
        return;
    }
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

void cleanup() 
{
    free_scheduled_tasks(SIZE_MAX); 

    while (ioopm_linked_list_size(get_schedule_linked_list()) > 0) 
    {
        bool successful = false;
        obj *object = ioopm_linked_list_get(get_schedule_linked_list(), 0, &successful).p;

        if (successful && object) 
        {
            memdata_t *metadata = GET_METADATA(object);
            if (metadata && metadata->rc == 0) 
            {
                release_destructor(object);
                free(metadata);
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