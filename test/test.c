#include "../src/ref.h"
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

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

// void test2(void) {
//     CU_ASSERT_EQUAL(1 + 1, 2);
// }

// void test_get_memdata_ht(void) {    
//     ioopm_hash_table_t *ht_rc = get_memdata_ht();
//     CU_ASSERT_PTR_NOT_NULL(ht_rc);
//     ioopm_hash_table_t *ht_rc2 = get_memdata_ht();
//     CU_ASSERT_PTR_EQUAL(ht_rc, ht_rc2);
    
//     //ioopm_hash_table_destroy(ht_rc); //TODO swap for shutdown later?? //DANGLING POINTERS
// }

// void test_get_memdata_ht_retrieve(void) {
//     ioopm_hash_table_t *ht_rc = get_memdata_ht();
//     CU_ASSERT_PTR_NOT_NULL(ht_rc);
    
//     ioopm_option_t check = ioopm_hash_table_lookup(ht_rc, int_elem(4));
//     CU_ASSERT_FALSE(check.success);
    
    
//     //ioopm_hash_table_destroy(ht_rc); //TODO swap for shutdown later?? //DANGLING POINTERS
// }

// void dummy_destructor(void *ptr) {
//     free(ptr);
// }

// void memdata_destructor(void *ptr) {
//     free(ptr);
// }

// void test_memdata_generate(void) {
//     memdata_t *memdata = memdata_generate(memdata_destructor);
//     CU_ASSERT_PTR_NOT_NULL(memdata);
//     CU_ASSERT_EQUAL(memdata->rc, 0);
//     CU_ASSERT_EQUAL(memdata->destructor, memdata_destructor);
//     memdata->destructor(memdata); 
// }
// // currently leaks, need a special destructor for memdata
// void test_memdata_generate_insert_ht(void) {
//     memdata_t *memdata = memdata_generate(memdata_destructor);
//     ioopm_hash_table_insert(get_memdata_ht(), int_elem((int) &memdata), ptr_elem(memdata));
//     CU_ASSERT_PTR_NOT_NULL(memdata);
//     ioopm_option_t option = ioopm_hash_table_lookup(get_memdata_ht(), int_elem((int) &memdata));
//     CU_ASSERT_TRUE(option.success);
//     free(option.value.p);

    
//     ioopm_hash_table_insert(get_memdata_ht(), int_elem(2), ptr_elem(memdata_generate(memdata_destructor)));
//     ioopm_option_t option2 = ioopm_hash_table_lookup(get_memdata_ht(), int_elem(2));
//     CU_ASSERT_TRUE(option2.success);
//     option2 = ioopm_hash_table_lookup(get_memdata_ht(), int_elem(2));
//     free(option2.value.p);
//     CU_ASSERT_TRUE(option.success);

//     // TODO run cleanup at end of every test??
// }


// void test_add_to_schedule(){
//     ioopm_list_t *list = get_schedule_linked_list();
//     //Check if list doesn´t exist:
//     CU_ASSERT_PTR_NOT_NULL(list);
//     obj *object = malloc(sizeof(obj));

//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
//     // If list exist, add object to schedule:
//     add_to_schedule(object);
//     //Check if object was added
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 1);
//     add_to_schedule(object);
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 2);
//     free(object);
//     ioopm_linked_list_clear(list);
// }

// void test_free_scheduled_task_empty(){
//     set_cascade_limit(3);
//     ioopm_list_t *list= get_schedule_linked_list();
//     free_scheduled_tasks(5);
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
// }

// void test_free_scheduled_task_one_task(){
//     set_cascade_limit(3);
//     ioopm_list_t *list = get_schedule_linked_list();
//     obj *object = malloc(sizeof(obj));
//     add_to_schedule(object);
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 1);
//     free_scheduled_tasks(1);
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
//     free(object);

// }

// void test_free_scheduled_tasks_over_cascade(){
//     set_cascade_limit(3);
//     ioopm_list_t *list = get_schedule_linked_list();
//     obj *object = malloc(sizeof(obj));
//     add_to_schedule(object);
//     add_to_schedule(object);
//     add_to_schedule(object);
//     add_to_schedule(object);
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 4);
//     free_scheduled_tasks(4*sizeof(object));
  
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
    
//     free(object);
// }

// void test_free_scheduled_tasks_until_size(){

//     set_cascade_limit(100);
//     ioopm_list_t *list = get_schedule_linked_list();
//     obj *object = malloc(sizeof(obj));
//     add_to_schedule(object);
//     add_to_schedule(object);
//     add_to_schedule(object);
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 3);
//     free_scheduled_tasks(2*sizeof(object));
  
//     CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 1);
//     free(object);
    
// }


// void check_allocation(obj *object, function1_t expected_destructor) {
//     CU_ASSERT_PTR_NOT_NULL(object);
//     // Look up the metadata in the hash table
//     memdata_t *metadata = (memdata_t *)ioopm_hash_table_lookup(get_memdata_ht(), int_elem((int)(intptr_t)object)).value.p;
//     CU_ASSERT_EQUAL(metadata->rc, 0);
//     CU_ASSERT_EQUAL(metadata->destructor, expected_destructor);
//     free(metadata);  
// }

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

// Larger scale tests. Complex data structures first with given constructors, then the same, handled
// by the default destructor

// Binary trees
typedef struct node node_t;
struct node {
    int val;
    node_t *left;
    node_t *right;
};

void node_destroy(obj *object) {
    node_t *node = (node_t *) object;
    release(node->left); // release ignores null vales, so need to check
    release(node->right);
}

node_t *node_create(int val, node_t *left, node_t *right) {
    node_t *node = allocate(sizeof(node_t), node_destroy);
    retain(node); // TODO not sure if necessary
    node->val = val;
    node->left = left;
    node->right = right;
    return node;
}

void test_binary_tree_given_destructor() {
    set_cascade_limit(10);
    
    // Create a binary tree struct with the library's functions
    /* Tree 1
        4
      2   3
    1
    */
    node_t *n1 = node_create(1, NULL, NULL);
    node_t *n2 = node_create(2, n1, NULL);
    node_t *n3 = node_create(3, NULL, NULL);
    node_t *n4 = node_create(4, n2, n3);

    // Create a copy of node 2
    node_t *n2_copy = n2;
    retain(n2);

    // Releases node 4, and with it node 3, but node 2 (and 1) remains since rf > 0
    release(n4);

    // node 2 and 1 should still be reachable
    CU_ASSERT_EQUAL(n2_copy->val, 2);
    CU_ASSERT_EQUAL(n2_copy->left->val, 1);

    release(n2_copy);
    CU_ASSERT_EQUAL(rc(n2_copy), 0); // TODO might not be reachable, can check if its 1 before
    // releasing instead
}

void test_binary_tree_default_destructor() {
    CU_ASSERT_TRUE(1);
    // TODO fixe the previous one first, then it should be the same but with NULL as destructor
}

typedef struct weird_array weird_array_t;
struct weird_array {
    int i1;
    int i2;
    int i3;
    char *i4;
    int i5;
    weird_array_t *i6;
    int i7;
};

void str_non_destructor(obj *object) {
    return;
}

void weird_array_destroy(obj *object) {
    weird_array_t *arr = (weird_array_t *) object;
    release(arr->i4);
    release(arr->i6);
}

weird_array_t *weird_array_create(char *i4, weird_array_t *i6) {
    weird_array_t *arr = allocate(sizeof(weird_array_t), weird_array_destroy);
    retain(arr);
    arr->i1 = 1;
    arr->i2 = 2;
    arr->i3 = 3;
    arr->i4 = allocate_array(strlen(i4), sizeof(char*), str_non_destructor); // TODO not sure if it should be char or char*
    retain(i4);
    arr->i5 = 5;
    arr->i6 = i6;
    arr->i7 = 7;
    return arr;
}

void test_array_struct_given_destructor() {
    char *str1 = "One";
    weird_array_t *arr1 = weird_array_create(str1, NULL);
    char *str2 = "Two";
    weird_array_t *arr2 = weird_array_create(str2, arr1);
    CU_ASSERT_EQUAL(arr1->i1, arr2->i6->i1);
    CU_ASSERT_EQUAL(str1, arr2->i6->i4);

    release(arr1);
    release(arr2);
}

void test_array_struct_default_destructor() {
    CU_ASSERT_TRUE(1);
    // TODO fixe the previous one first, then it should be the same but with NULL as destructor
}

int main() {
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    // CU_pSuite unit_test_suite1 = CU_add_suite("Unit tests for allocate, deallocate, retain, release & rc", init_suite, clean_suite);
    // if (unit_test_suite1 == NULL) {
    //     // If the test suite could not be added, tear down CUnit and exit
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    CU_pSuite integration_test_suite = CU_add_suite("Integration tests with complex data structures, given destructors and the default destructor", init_suite, clean_suite);
    if (integration_test_suite == NULL) {
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
        // (CU_add_test(unit_test_suite1, "Get memdata", test_get_memdata_ht) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Get memdata", test_get_memdata_ht_retrieve) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Memdata generate", test_memdata_generate) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Memdata generate_insert", test_memdata_generate_insert_ht) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Add to schedule", test_add_to_schedule) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Free schedule when it is empty", test_free_scheduled_task_empty) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Free schedule with one task", test_free_scheduled_task_one_task) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Free schedule that goes over cascade limit", test_free_scheduled_tasks_over_cascade) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Free schedule that goes until size limit", test_free_scheduled_tasks_until_size) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Allocate", test_allocate) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Allocate array", test_allocate_array) == NULL) ||
        // Test suite 2
        // (CU_add_test(integration_test_suite, "Binary tree struct with destructor", test_binary_tree_given_destructor) == NULL) ||
        // (CU_add_test(integration_test_suite, "Binary tree struct without destructor", test_binary_tree_default_destructor) == NULL) ||
        (CU_add_test(integration_test_suite, "Weird array struct with destructor", test_array_struct_given_destructor) == NULL) ||
        (CU_add_test(integration_test_suite, "Weird array struct without destructor", test_array_struct_default_destructor) == NULL) ||
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