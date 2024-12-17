#include "refmem.h"
#include "inlupp2/generic_data_structures/hash_table.h"
#include "inlupp2/generic_data_structures/common.h"

bool int_eq(elem_t e1, elem_t e2)
{
    return e1.i == e2.i;
}

bool ptr_eq(elem_t e1, elem_t e2)
{
    return e1.p == e2.p;
}

ioopm_hash_table_t *ht_rc = ioopm_hash_table_create(int_eq, ptr_eq, );
int CASCADE_LIMIT = 100;
typedef struct memdata memdata_t;
struct memdata{
    size_t rc;
    function1_t destructor;
};





obj *allocate(size_t size, function1_t *destructor) {
    free_scheduled_tasks(size);
    obj *object = malloc(size);
    ioopm_hash_table_insert(ht_rc, int_elem((int) object), ptr_elem(memdata_generate(destructor)));
}

obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor) {
    free_scheduled_tasks(elem_size);
    obj *object = malloc(elements * elem_size);
    ioopm_hash_table_insert(ht_rc, int_elem((int) object), ptr_elem(memdata_generate(destructor)));
}

memdata_t *memdata_generate(function1_t *destructor) {
    memdata_t *memdata = calloc(1, sizeof(memdata_t));
    memdata->rc = 0;
    memdata->destructor = destructor;
    return memdata;
}

void release(obj *object) {
    if (object != NULL) {
        memdata_t *memdata = (memdata_t *) ioopm_hash_table_lookup(ht_rc, int_elem((int) object)).value.p; // Change to macro, and maybe check for success
        if (memdata->rc == 0) {
            add_to_schedule(object);
        } else {
            memdata->rc--;
        }
    }
    free_scheduled_tasks(0);
}

free_scheduled_tasks(size) {
    size_t freed_size = 0;
    for (int i = 0; i < CASCADE_LIMIT; i++) {
        obj *to_remove = list_get(i);
        release_destructor(to_remove);
        linked_list_remove(i);
        ioopm_hash_table_remove(ht_rc, int_elem((int) to_remove));
    }
}

release_destructor(to_remove) {
    return destructor();
}

cleanup() {
    free_scheduled_tasks(sizeof(scheduled_list));
    // for all entries in ht where rc = 0 call release
    // remove the aforementioned entries from ht
}

shutdown() {
    free_all();
}

free_all() {
    hash_table_destroy(ht_rc);
    linked_list_destroy(scheduled_list);
    free_scheduled_tasks(size_of(scheduled_list));
}

size_t get_cascade_limit(){
    return CASCADE_LIMIT;
}

void set_cascade_limit(size_t size){
    CASCADE_LIMIT = size;
}