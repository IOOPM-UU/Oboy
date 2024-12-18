#include "ref.h"
int CASCADE_LIMIT = 100;

bool int_eq(elem_t e1, elem_t e2)
{
    return e1.i == e2.i;
}

bool ptr_eq(elem_t e1, elem_t e2)
{
    return e1.p == e2.p;
}

static int default_hash_function(elem_t value)
{
    return value.i;
}

typedef struct memdata memdata_t;
struct memdata{
    size_t rc;
    function1_t destructor;
};

ioopm_hash_table_t *get_memdata_ht(){
    static ioopm_hash_table_t *memdata = NULL;
    if(memdata == NULL){
        memdata = ioopm_hash_table_create(int_eq, NULL, default_hash_function);
    }
    return memdata;
}

obj *allocate(size_t bytes, function1_t destructor) {
    free_scheduled_tasks(bytes);
    obj *obj = malloc(bytes);
    ioopm_hash_table_insert(get_memdata_ht, int_elem(&obj), ptr_elem(memdata_generate(destructor)));
}

memdata_t *memdata_generate(function1_t destructor) {
    memdata_t *memdata = calloc(1, sizeof(memdata_t));
    memdata->rc = 0;
    memdata->destructor = destructor;
    return memdata;
}

obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor) {
    free_scheduled_tasks(elem_size);
    obj *obj = calloc(elements, elem_size);
    ioopm_hash_table_insert(get_memdata_ht, int_elem(&obj), ptr_elem(memdata_generate(destructor)));
}

void release(obj *object) {
    if (object != NULL) {
        memdata_t *memdata = (memdata_t *) ioopm_hash_table_lookup(ht_rc, int_elem((int) object)).value.p; // Change to macro, and maybe check for success
        if (memdata->rc == 0) {
            add_to_schedule(object);
        } else {
            memdata->rc --;
        }
       free_scheduled_tasks();
    }
}

void add_to_schedule(obj *obj) {
    ioopm_linked_list_append(schedule_list, obj);
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
