#include "ref.h"

int CASCADE_LIMIT;

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

// typedef struct memdata memdata_t;


ioopm_hash_table_t *get_memdata_ht() {
    static ioopm_hash_table_t *memdata_ht = NULL;
    if(memdata_ht == NULL){
        memdata_ht = ioopm_hash_table_create(int_eq, NULL, default_hash_function);
    }
    return memdata_ht;
}
ioopm_list_t *get_schedule_linked_list(){
    static ioopm_list_t *schedule_linked_list = NULL;
    if(schedule_linked_list == NULL){
        schedule_linked_list = ioopm_linked_list_create(int_eq);
    }
    return schedule_linked_list;
}

obj *allocate(size_t bytes, function1_t destructor) {
    free_scheduled_tasks(bytes);
    obj *obj = malloc(bytes);
    ioopm_hash_table_insert(get_memdata_ht(), int_elem(&obj), ptr_elem(memdata_generate(destructor)));
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
    ioopm_hash_table_insert(get_memdata_ht(), int_elem(&obj), ptr_elem(memdata_generate(destructor)));
}

void release(obj *object) {
    if (object != NULL) {
        memdata_t *memdata = (memdata_t *) ioopm_hash_table_lookup(get_memdata_ht(), int_elem((int) object)).value.p; // Change to macro, and maybe check for success
        if (memdata->rc == 0) {
            add_to_schedule(object);
        } else {
            memdata->rc --;
        }
       free_scheduled_tasks(0); //TODO: ska ej vara 0 sen
    }
}

//hjälp funktion
void add_to_schedule(obj *object) {
    ioopm_linked_list_append(get_schedule_linked_list(), ptr_elem(object));
}


//hjälp funktion
void free_scheduled_tasks(size_t size) {
    size_t freed_size = 0;
    bool successful1 = NULL;
    bool successful2 = NULL;
    for (int i = 0; i < get_cascade_limit() || freed_size <= size ; i++) {
        obj *to_remove = ioopm_linked_list_get(get_schedule_linked_list(), 0, &successful1).p;
        freed_size+=sizeof(to_remove);
        release_destructor(to_remove);
        ioopm_linked_list_remove(get_schedule_linked_list(), 0, &successful2);
        ioopm_hash_table_remove(get_memdata_ht(), int_elem((int) to_remove));
        if (freed_size >=size){
            break;
        }
    }
}

void release_destructor(to_remove) {
    //return destructor(); // TODO
}

void cleanup() {
    free_scheduled_tasks(sizeof(get_schedule_linked_list()));
    // for all entries in ht where rc = 0 call release
    // remove the aforementioned entries from ht
}

void shutdown() {
    free_all();
}

void free_all() {
    // ioopm_hash_table_clear(get_memdata_ht());
    ioopm_hash_table_destroy(get_memdata_ht());
    ioopm_linked_list_destroy(get_schedule_linked_list());
    
}

size_t get_cascade_limit(){
    return CASCADE_LIMIT;
}

void set_cascade_limit(size_t size){
    CASCADE_LIMIT = size;
}
