#pragma once
#include <stdlib.h>
#include <stddef.h>
#include "inlupp2_DONOTTOUCH/generic_data_structures/hash_table.h"
#include "inlupp2_DONOTTOUCH/generic_data_structures/linked_list.h"
#include "inlupp2_DONOTTOUCH/business_logic/common.h"

typedef void obj;
typedef void(*function1_t)(obj *);

struct memdata{
    size_t rc;
    function1_t destructor;
};
typedef struct memdata memdata_t;

 

void retain(obj *);
void release(obj *);
size_t rc(obj *);
/**
 * @brief Allocates memory and associates it with a destructor function. 
 * Also calls free schedule tasks
 *
 * This function allocates a block of memory of the specified size using malloc,
 * initializes a memdata structure to manage the memory, and associates the
 * memory block with a destructor function. The memdata structure is used to
 * keep track of the reference count and the destructor function. It calls free 
 * scheduled tasks which deallocates memory that has been scheduled for deallocation.
 *
 * @param bytes The size of the memory block to allocate, in bytes.
 * @param destructor A function pointer to the destructor function that will
 *                   be called when the memory is deallocated.
 * @return A pointer to the allocated memory block.
 */
obj *allocate(size_t bytes, function1_t destructor);
obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor);

/// @brief Retrieves the static hashtable with allocated memdata. Creates the hashtable the first
/// time the function is called, later calls returns the pointer. (Free'd by shutdown)
/// @return returns the static hashtable we have in global.
ioopm_hash_table_t *get_memdata_ht();

/**
 * @brief Allocates and initializes a memdata structure.
 *
 * This function allocates memory for a memdata structure using calloc,
 * initializes the reference count (rc) to 0, and sets the destructor
 * function to the provided function pointer.
 *
 * @param destructor A function pointer to the destructor function that will
 *                   be called when the memory is deallocated.
 * @return A pointer to the newly allocated and initialized memdata structure.
 */
memdata_t *memdata_generate(function1_t destructor);

void deallocate(obj *);
void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();
ioopm_list_t *get_schedule_linked_list();
void free_scheduled_tasks(size_t size);
void add_to_schedule(obj *object);