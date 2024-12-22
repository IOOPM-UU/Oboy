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

obj *allocate(size_t bytes, function1_t destructor);
obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor);

/// @brief Retrieves the static hashtable with allocated memdata. Creates the hashtable the first
/// time the function is called, later calls returns the pointer. (Free'd by shutdown)
/// @return returns the static hashtable we have in global.
ioopm_hash_table_t *get_memdata_ht();

/*
* @brief Callocs a memdata struct and sets the rc to 0 
*    and the destructor to the given function

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