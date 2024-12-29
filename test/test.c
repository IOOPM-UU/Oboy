#include "../src/ref.h"
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../src/inlupp2_DONOTTOUCH/generic_data_structures/linked_list.h"
#include <assert.h>
#include <limits.h>

int init_suite(void) {
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void) {
    // Change this function if you want to do something *after* you
    // run a test suite
    shutdown();
    return 0;
}

// Unit tests

// deallocate

// allocate
// void test_alloc(void) {
//     obj_t *obj = NULL;
//     CU_ASSERT_PTR_NULL(obj);
//     obj = allocate(sizeof(obj));
//     CU_ASSERT_PTR_NOT_NULL(obj);
//     deallocate(obj);
//     CU_ASSERT_PTR_NULL(obj);
// }

void test2(void) {
    CU_ASSERT_EQUAL(1 + 1, 2);
}
/*
void test_get_memdata_ht(void) {    
    ioopm_hash_table_t *ht_rc = get_memdata_ht();
    CU_ASSERT_PTR_NOT_NULL(ht_rc);
    ioopm_hash_table_t *ht_rc2 = get_memdata_ht();
    CU_ASSERT_PTR_EQUAL(ht_rc, ht_rc2);
    
    //ioopm_hash_table_destroy(ht_rc); //TODO swap for shutdown later?? //DANGLING POINTERS
}

void test_get_memdata_ht_retrieve(void) {
    ioopm_hash_table_t *ht_rc = get_memdata_ht();
    CU_ASSERT_PTR_NOT_NULL(ht_rc);
    
    ioopm_option_t check = ioopm_hash_table_lookup(ht_rc, int_elem(4));
    CU_ASSERT_FALSE(check.success);
    
    
    //ioopm_hash_table_destroy(ht_rc); //TODO swap for shutdown later?? //DANGLING POINTERS
} */

void dummy_destructor(void *ptr) {
    free(ptr);
}

void memdata_destructor(void *ptr) {
    free(ptr);
}

/* void default_destructor(obj *object) {
    if (!object) return;

    memdata_t *metadata = (memdata_t *)((char *)object - sizeof(memdata_t));
    size_t obj_size = metadata->size;

    for (size_t offset = 0; offset + sizeof(void *) <= obj_size; offset += sizeof(void *)) {
        void **possible_pointer = (void **)((char *)object + offset);

        // Check if the address is in the allocation tracker
        if (ioopm_hash_table_lookup(get_memdata_ht(), ptr_elem(*possible_pointer)).success) {
            release(*possible_pointer);
        }
    }
} */
/* void test_memdata_generate(void) {
    memdata_t *memdata = memdata_generate(memdata_destructor);
    CU_ASSERT_PTR_NOT_NULL(memdata);
    CU_ASSERT_EQUAL(memdata->rc, 0);
    CU_ASSERT_EQUAL(memdata->destructor, memdata_destructor);
    memdata->destructor(memdata); 
} */
// currently leaks, need a special destructor for memdata
/* void test_memdata_generate_insert_ht(void) {
    memdata_t *memdata = memdata_generate(memdata_destructor);
    ioopm_hash_table_insert(get_memdata_ht(), int_elem((int) &memdata), ptr_elem(memdata));
    CU_ASSERT_PTR_NOT_NULL(memdata);
    ioopm_option_t option = ioopm_hash_table_lookup(get_memdata_ht(), int_elem((int) &memdata));
    CU_ASSERT_TRUE(option.success);
    free(option.value.p);

    
    ioopm_hash_table_insert(get_memdata_ht(), int_elem(2), ptr_elem(memdata_generate(memdata_destructor)));
    ioopm_option_t option2 = ioopm_hash_table_lookup(get_memdata_ht(), int_elem(2));
    CU_ASSERT_TRUE(option2.success);
    option2 = ioopm_hash_table_lookup(get_memdata_ht(), int_elem(2));
    free(option2.value.p);
    CU_ASSERT_TRUE(option.success);

    // TODO run cleanup at end of every test??
} */


/* void test_add_to_schedule(){
    ioopm_list_t *list = get_schedule_linked_list();
    //Check if list doesn´t exist:
    CU_ASSERT_PTR_NOT_NULL(list);
    obj *object = malloc(sizeof(obj));

    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
    // If list exist, add object to schedule:
    add_to_schedule(object);
    //Check if object was added
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 1);
    add_to_schedule(object);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 2);
    free_scheduled_tasks(2*sizeof(object));
    ioopm_linked_list_clear(list);
}
 */
/* void test_free_scheduled_task_empty(){
    set_cascade_limit(3);
    ioopm_list_t *list= get_schedule_linked_list();
    free_scheduled_tasks(5);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
}

void test_free_scheduled_task_one_task(){
    set_cascade_limit(3);
    ioopm_list_t *list = get_schedule_linked_list();
    obj *object = allocate(sizeof(obj), dummy_destructor);
    add_to_schedule(object);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 1);
    free_scheduled_tasks(1);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
    //free(object);
    free_scheduled_tasks(sizeof(object));

} */

/* void test_free_scheduled_tasks_over_cascade(){
    set_cascade_limit(3);
    ioopm_list_t *list = get_schedule_linked_list();
    obj *object = malloc(sizeof(obj));
    add_to_schedule(object);
    add_to_schedule(object);
    add_to_schedule(object);
    add_to_schedule(object);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 4);
    free_scheduled_tasks(4*sizeof(object));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
}

void test_free_scheduled_tasks_until_size(){

    set_cascade_limit(100);
    ioopm_list_t *list = get_schedule_linked_list();
    obj *object = malloc(sizeof(obj));
    add_to_schedule(object);
    add_to_schedule(object);
    add_to_schedule(object);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 3);
    free_scheduled_tasks(2*sizeof(object));
  
    CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 1);
    //free(object);
    
}

 */
void check_allocation(obj *object, function1_t expected_destructor) {
    CU_ASSERT_PTR_NOT_NULL(object); // Ensure object is not null

    // Access metadata via pointer arithmetic
    metadata_t *metadata = GET_METADATA(object);

    CU_ASSERT_EQUAL(metadata->rc, 0);                      // Verify reference count
    CU_ASSERT_EQUAL(metadata->destructor, expected_destructor); // Verify destructor
    CU_ASSERT(metadata->size > 0);                         // Ensure valid size
}

// void test_allocate() {
//     // Test 1: Allocate memory with no destructor
//     obj *object = allocate(100, NULL);
//     check_allocation(object, NULL);

//     // Test 2: Allocate memory with a custom destructor
//     object = allocate(100, dummy_destructor);
//     check_allocation(object, dummy_destructor);
// }

// void test_allocate_array() {
//     // Test 1: Allocate an array with no destructor
//     obj *object = allocate_array(10, sizeof(int), NULL);
//     check_allocation(object, NULL);

//     // Test 2: Allocate an array with a custom destructor
//     object = allocate_array(10, sizeof(int), dummy_destructor);
//     check_allocation(object, dummy_destructor);
// }
/* void test_default_destructor() {
    set_cascade_limit(10);

    // Create nodes
    link_t *link1 = allocate(sizeof(link_t), default_destructor);
    link_t *link2 = allocate(sizeof(link_t), default_destructor);

    // Link nodes
    link1->next = link2;
    link2->next = NULL;

    // Call release on the head node
    release(link1);

    // Check that both nodes are properly deallocated
    assert(ioopm_hash_table_lookup(get_memdata_ht(), ptr_elem(link1)).success == false);
    assert(ioopm_hash_table_lookup(get_memdata_ht(), ptr_elem(link2)).success == false);

    printf("Test Case 1 passed: Default destructor released all linked pointers.\n");
} */
void test_allocate_and_free_scheduled_tasks(void)
{
    // Set cascade limit
    set_cascade_limit(3);

    // 1. Allocate memory blocks
    obj *object1 = allocate(100, NULL);
    CU_ASSERT_PTR_NOT_NULL(object1);

    obj *object2 = allocate(200, NULL);
    CU_ASSERT_PTR_NOT_NULL(object2);

    obj *object3 = allocate(300, NULL);
    CU_ASSERT_PTR_NOT_NULL(object3);

    // 2. Add all to schedule
    add_to_schedule(object1);
    add_to_schedule(object2);
    add_to_schedule(object3);

    // We should have 3
    CU_ASSERT_EQUAL(ioopm_linked_list_size(get_schedule_linked_list()), 3);

    // 3. Free tasks up to 150 bytes => frees object1(100)
    free_scheduled_tasks(150);
    // => now 2 remain
    CU_ASSERT_EQUAL(ioopm_linked_list_size(get_schedule_linked_list()), 1);

    // 4. Free tasks up to 600 => enough for object2(200) + object3(300)
    free_scheduled_tasks(600);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(get_schedule_linked_list()), 0);

    // NOTE: no call to free_all() here if your suite or test teardown calls it.
    // Otherwise you can do:
    // free_all();
}
typedef struct link link_t;

struct link
{
    elem_t value;
    link_t *previous;
    link_t *next;
};


#define null_elem \
    (elem_t) { 0 }

// Helper function for testing on links
static link_t *link_create_with_allocate(elem_t value, link_t *previous, link_t *next)
{
    link_t *link = allocate(sizeof(link_t), NULL);
    link->previous = previous;
    link->next = next;
    link->value = value;
    return link;
}


void test_allocate_links_and_free_scheduled_tasks(void)
{
    // Set cascade limit
    set_cascade_limit(3);

    // 1. Allocate memory blocks
    link_t *link1 = link_create_with_allocate(null_elem, NULL, NULL);
    CU_ASSERT_PTR_NOT_NULL(link1);

    link_t *link2 = link_create_with_allocate(null_elem, NULL, NULL);
    CU_ASSERT_PTR_NOT_NULL(link2);

    link_t *link3 = link_create_with_allocate(null_elem, NULL, NULL);
    CU_ASSERT_PTR_NOT_NULL(link3);

    // 2. Add all to schedule
    add_to_schedule(link1);
    add_to_schedule(link2);
    add_to_schedule(link3);

    // We should have 3
    CU_ASSERT_EQUAL(ioopm_linked_list_size(get_schedule_linked_list()), 3);

    // 3. Free tasks up to 150 bytes => frees object1(100)
    free_scheduled_tasks(sizeof(link_t));
    // => now 2 remain
    CU_ASSERT_EQUAL(ioopm_linked_list_size(get_schedule_linked_list()), 2);

    // 4. Free tasks up to 600 => enough for object2(200) + object3(300)
    free_scheduled_tasks(2 * sizeof(link_t));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(get_schedule_linked_list()), 0);

    // NOTE: no call to free_all() here if your suite or test teardown calls it.
    // Otherwise you can do:
    // free_all();
}



void test_allocate_array_then_free(void)
{
    // Set cascade limit
    set_cascade_limit(3);

    // 1. Allocate memory blocks
    obj *object1 = allocate_array(10, 100, NULL);
    CU_ASSERT_PTR_NOT_NULL(object1);

    obj *object2 = allocate_array(10, 100, NULL);
    CU_ASSERT_PTR_NOT_NULL(object2);

    obj *object3 = allocate_array(10, 100, NULL);
    CU_ASSERT_PTR_NOT_NULL(object3);

    // 2. Add all to schedule
    add_to_schedule(object1);
    add_to_schedule(object2);
    add_to_schedule(object3);

    free_scheduled_tasks(INT_MAX);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(get_schedule_linked_list()), 0);

    // NOTE: no call to free_all() here if your suite or test teardown calls it.
    // Otherwise you can do:
    // free_all();
}

void test_allocate_strings_then_free(void)
{
    // Set cascade limit
    set_cascade_limit(3);

    // 1. Allocate memory blocks
    char *str1 = allocate_array(5, sizeof(char), NULL);
    for (int i = 0; i < 5; i++) {
        str1[i] = "a";
    }
    CU_ASSERT_PTR_NOT_NULL(str1);
    
    char *str2 = allocate_array(6, sizeof(char), NULL);
    for (int i = 0; i < 6; i++) {
        str2[i] = "b";
    }
    CU_ASSERT_PTR_NOT_NULL(str2);
    
    char *str3 = allocate_array(7, sizeof(char), NULL);
    for (int i = 0; i < 7; i++) {
        str3[i] = "c";
    }
    CU_ASSERT_PTR_NOT_NULL(str3);

    // 2. Add all to schedule (with release this time)
    release(str1);
    release(str2);
    release(str3);

    free_scheduled_tasks(INT_MAX);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(get_schedule_linked_list()), 0);

    // NOTE: no call to free_all() here if your suite or test teardown calls it.
    // Otherwise you can do:
    // free_all();
}



int main() {
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite unit_test_suite1 = CU_add_suite("Unit tests for allocate, deallocate, retain, release & rc", init_suite, clean_suite);
    if (unit_test_suite1 == NULL) {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // This is where we add the test functions to our test suite.
    // For each call to CU_add_test we specify the test suite, the
    // name or description of the test, and the function that runs
    // the test in question. If you want to add another test, just
    // copy a line below and change the information
    if (
        //(CU_add_test(unit_test_suite1, "Add to schedule", test_add_to_schedule) == NULL) ||
        //(CU_add_test(unit_test_suite1, "Free schedule when it is empty", test_free_scheduled_task_empty) == NULL) ||
        //(CU_add_test(unit_test_suite1, "Free schedule with one task", test_free_scheduled_task_one_task) == NULL) ||
        //(CU_add_test(unit_test_suite1, "Free schedule that goes over cascade limit", test_free_scheduled_tasks_over_cascade) == NULL) ||
        //(CU_add_test(unit_test_suite1, "Free schedule that goes until size limit", test_free_scheduled_tasks_until_size) == NULL) ||
        //(CU_add_test(unit_test_suite1, "Allocate", test_allocate) == NULL) ||
        //(CU_add_test(unit_test_suite1, "Allocate array", test_allocate_array) == NULL) ||
        //(CU_add_test(unit_test_suite1, "Default destructor", test_default_destructor) == NULL) ||
        //(CU_add_test(unit_test_suite1, "Free scheduled tasks with allocate", test_free_scheduled_tasks_with_allocate) == NULL) ||
        (CU_add_test(unit_test_suite1, "Allocate and free scheduled tasks", test_allocate_and_free_scheduled_tasks) == NULL) ||
        (CU_add_test(unit_test_suite1, "Allocate and free scheduled tasks", test_allocate_links_and_free_scheduled_tasks) == NULL) ||
        (CU_add_test(unit_test_suite1, "Allocate and free array scheduled tasks", test_allocate_array_then_free) == NULL) ||
        (CU_add_test(unit_test_suite1, "Allocate and free string scheduled tasks", test_allocate_strings_then_free) == NULL) ||
        0
    ) 
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
} 