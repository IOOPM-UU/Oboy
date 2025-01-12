#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "lib/lib_hash_table.h"
#include "lib/lib_linked_list.h"
#include "lib/lib_common.h"


// Void pointer used for allocation
typedef void obj;

// A destructor function pointer
typedef void(*function1_t)(obj *);

// The data we store when allocating a object
struct metadata {
    uint8_t rc;                      // Reference count
    function1_t destructor;          // Function to call before freeing
    size_t size;                     // Total size of the user allocation
};
typedef struct metadata metadata_t;

/**
 * @brief Increases the reference count for a given object.
 * 
 * @param object A pointer to the object whose reference count should be incremented.
 */
void retain(obj *object);

/**
 * @brief Decreases the reference count for a given object and deallocates it if the count reaches zero.
 * 
 * @param object A pointer to the object whose reference count should be decremented.
 */
void release(obj *object);

/**
 * @brief Retrieves the current reference count of an object.
 * 
 * @param object A pointer to the object.
 * @return The reference count of the object, or 0 if the object has no metadata.
 */
uint8_t rc(obj *object);

/**
 * @brief Allocates memory for an object and associates it with metadata.
 * 
 * @param bytes The number of bytes to allocate for the object.
 * @param destructor A function to be called before deallocating the object.
 * @return A pointer to the allocated object, or NULL if allocation fails.
 */
obj *allocate(size_t bytes, function1_t destructor);

/**
 * @brief Allocates memory for an array of objects and associates it with metadata.
 * 
 * @param elements The number of elements in the array.
 * @param elem_size The size of each element in bytes.
 * @param destructor A function to be called before deallocating the object.
 * @return A pointer to the allocated array, or NULL if allocation fails.
 */
obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor);

/**
 * @brief Generates metadata for an object.
 * 
 * @param destructor A function to be called before deallocating the object.
 * @param size The size of the object.
 * @return A pointer to the generated metadata, or NULL if allocation fails.
 */
metadata_t *metadata_generate(function1_t destructor, size_t size);

/**
 * @brief Deallocates an object by adding it to the schedule and triggering destructors as needed.
 * 
 * @param object A pointer to the object to deallocate.
 */
void deallocate(obj *object);

/**
 * @brief Sets the cascade limit for freeing objects.
 * 
 * @param size The new cascade limit.
 */
void set_cascade_limit(size_t size);

/**
 * @brief Retrieves the current cascade limit for freeing objects.
 * 
 * @return The current cascade limit.
 */
size_t get_cascade_limit();

/**
 * @brief Cleans up scheduled tasks by freeing all eligible objects.
 */
void cleanup();

/**
 * @brief Shuts down the memory manager by freeing all resources and clearing metadata.
 */
void shutdown();

/**
 * @brief Retrieves the global metadata hash table.
 * 
 * @return A pointer to the metadata hash table.
 */
lib_hash_table_t *get_metadata_ht();

/**
 * @brief Retrieves the global schedule linked list.
 * 
 * @return A pointer to the schedule linked list.
 */
lib_list_t *get_schedule_linked_list();



/**
 * @brief Adds an object to the schedule for later deallocation.
 * 
 * @param object A pointer to the object to schedule.
 */
void add_to_schedule(obj *object);

/**
 * @brief Frees all resources and clears all metadata and scheduled tasks.
 */
void free_all();

/**
 * @brief Creates a new reference-counted copy of the input string.
 * 
 * @param src A pointer to the source string to duplicate.
 * @return A pointer to the duplicated string.
 */
char *rc_strdup(char *src);

/**
 * @brief A dummy destructor for strings.
 * 
 * @param o A pointer to the object.
 */
void str_dummy_destructor(obj *o);