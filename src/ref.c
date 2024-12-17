#include "refmem.h"
int CASCADE_LIMIT = 100;
struct memdata{
    size_t rc;
    function1_t destructor;
}



obj *allocate(size_t size, function1_t *destructor) {
    free_scheduled_tasks(size);
    obj *obj = malloc(size);
    ioopm_hash_table_insert(ht_rc, &obj, memdata_generate(destructor));
}

memdata_t memdata_generate(function1_t *destructor) {
    memdata_t memdata = calloc(1, sizeof(memdata_t));
    memdata->rc = 0;
    memdata->destructor = destructor;
    return memdata;
}

void release(obj * obj) {
    if (obj != NULL) {
        if (rc == 0) {
            add_to_schedule(obj);
        } else {
            rc--;
        }
    }
    free_scheduled_tasks();
}

free_scheduled_tasks(size) {
    size_t freed_size = 0;
    for (int i = 0; i < CASCADE_LIMIT; i++) {
        to_remove = list_get(i);
        release_destructor(to_remove);
        linked_list_remove(0);
        ht_remove()
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