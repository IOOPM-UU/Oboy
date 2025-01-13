#include "../src/refmem.h"
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../src/lib/lib_linked_list.h"
#include <assert.h>
#include <limits.h>

struct cell {
  struct cell *cell;
  int i;
  char *string;
};

typedef struct lib_link lib_link_t;

struct lib_link {
    lib_elem_t value;
    lib_link_t *previous;
    lib_link_t *next;
};

void cell_destructor(obj *c) { //kanske borde returna Size på det vi tagit bort?
    release(((struct cell *) c)->cell);
}

int init_suite(void) {
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void) {
    // Change this function if you want to do something *after* you
    // run a test suite
    // shutdown();
    lib_linked_list_destroy(get_schedule_linked_list());
    lib_hash_table_destroy(get_metadata_ht());
    return 0;
}

// Unit tests

void test_allocate_and_deallocate(void) {
    // Set cascade limit
    set_cascade_limit(1);

    // 1. Allocate memory blocks
    obj *object1 = allocate(100, NULL);
    CU_ASSERT_PTR_NOT_NULL(object1);

    obj *object2 = allocate(200, NULL);
    CU_ASSERT_PTR_NOT_NULL(object2);

    obj *object3 = allocate(300, NULL);
    CU_ASSERT_PTR_NOT_NULL(object3);

    // 2. We deallocate our objects, should not leak! No way to test with C_UNIT
    deallocate(object1);
    deallocate(object2);
    deallocate(object3);
}

void test_retain_release() {
    struct cell *c = allocate(sizeof(struct cell), cell_destructor);
    c->cell = NULL; // otherwise unconditional jump!
    CU_ASSERT_TRUE(rc(c) == 0);
    retain(c);
    CU_ASSERT_TRUE(rc(c) == 1);
    retain(c);
    CU_ASSERT_TRUE(rc(c) == 2);
    release(c);
    CU_ASSERT_TRUE(rc(c) == 1);
    release(c);
    CU_ASSERT_TRUE(rc(c) == 0); // cant find metadata
}

void test_retain_release2() {
    set_cascade_limit(5);
    struct cell *c = allocate(sizeof(struct cell), cell_destructor);
    CU_ASSERT_TRUE(rc(c) == 0);
    retain(c);
    CU_ASSERT_TRUE(rc(c) == 1);

    c->cell = allocate(sizeof(struct cell), cell_destructor);
    CU_ASSERT_TRUE(rc(c->cell) == 0);
    retain(c->cell);
    CU_ASSERT_TRUE(rc(c->cell) == 1);

    c->cell->cell = NULL;

    release(c);

}

void test_retain_release3() {
    set_cascade_limit(5);
    struct cell *c = allocate(sizeof(struct cell), cell_destructor);
    CU_ASSERT_TRUE(rc(c) == 0);
    retain(c);
    CU_ASSERT_TRUE(rc(c) == 1);

    c->cell = allocate(sizeof(struct cell), cell_destructor);
    CU_ASSERT_TRUE(rc(c->cell) == 0);
    retain(c->cell);
    struct cell *c2 = c->cell;
    retain(c2);
    CU_ASSERT_TRUE(rc(c->cell) == 2);

    c->cell->cell = NULL;

    release(c);
    release(c2);

}

void test_rc(void) {
    struct cell *c = allocate(sizeof(struct cell), cell_destructor); //allocate a cell
    c->i = 5;
    c->string = "cell";
    c->cell = NULL;

    CU_ASSERT_TRUE(rc(c) == 0);
    retain(c);
    CU_ASSERT_TRUE(rc(c) == 1);
    release(c);
}

void test_cascade_limit() {
    set_cascade_limit(1);
    CU_ASSERT_EQUAL(get_cascade_limit(), 1);
}

void test_default_destructor() {
    set_cascade_limit(10);

    // Create nodes
    lib_link_t *link1 = allocate(sizeof(lib_link_t), NULL);
    retain(link1);
    link1->next = allocate(sizeof(lib_link_t), NULL);
    retain(link1->next);

    // Call release on the head node
    release(link1);
    CU_ASSERT_FALSE(lib_hash_table_lookup(get_metadata_ht(), lib_ptr_elem(link1)).success);

    //printf("Test Case 1 passed: Default destructor released all linked pointers.\n");
}

#define null_elem \
    (lib_elem_t) { 0 }

// Helper function for testing on links
static lib_link_t *link_create_with_allocate(lib_elem_t value, lib_link_t *previous, lib_link_t *next) {
    lib_link_t *link = allocate(sizeof(lib_link_t), NULL);
    link->previous = previous;
    link->next = next;
    retain(link->next);
    link->value = value;
    return link;
}


void test_allocate_and_deallocate_links(void) {
    // Set cascade limit
    set_cascade_limit(1);

    // 1. Allocate memory blocks
    lib_link_t *link1 = link_create_with_allocate(null_elem, NULL, NULL);
    CU_ASSERT_PTR_NOT_NULL(link1);

    lib_link_t *link2 = link_create_with_allocate(null_elem, NULL, NULL);
    CU_ASSERT_PTR_NOT_NULL(link2);

    lib_link_t *link3 = link_create_with_allocate(null_elem, NULL, NULL);
    CU_ASSERT_PTR_NOT_NULL(link3);

    // 2. Add all to schedule
    deallocate(link1);
    deallocate(link2);
    deallocate(link3);
}

void test_allocate_array_then_deallocate(void) {
    // Set cascade limit
    set_cascade_limit(3);

    // 1. Allocate memory blocks
    obj *object1 = allocate_array(10, 100, NULL);
    CU_ASSERT_PTR_NOT_NULL(object1);

    obj *object2 = allocate_array(10, 100, NULL);
    CU_ASSERT_PTR_NOT_NULL(object2);

    obj *object3 = allocate_array(10, 100, NULL);
    CU_ASSERT_PTR_NOT_NULL(object3);

    //2. Deallocate all objects
    deallocate(object1);
    deallocate(object2);
    deallocate(object3);

    //should not leak when running tests!
}

void test_allocate_strings_then_release(void) {
    // Set cascade limit
    set_cascade_limit(1);

    // 1. Allocate memory blocks
    char *str1 = allocate_array(6, sizeof(char), str_dummy_destructor);
    for (int i = 0; i < 5; i++) {
        str1[i] = 'a';
    }
    CU_ASSERT_PTR_NOT_NULL(str1);
    
    char *str2 = allocate_array(7, sizeof(char), str_dummy_destructor);
    for (int i = 0; i < 6; i++) {
        str2[i] = 'b';
    }
    CU_ASSERT_PTR_NOT_NULL(str2);
    
    char *str3 = allocate_array(8, sizeof(char), str_dummy_destructor);
    for (int i = 0; i < 7; i++) {
        str3[i] = 'c';
    }
    CU_ASSERT_PTR_NOT_NULL(str3);

    // 2. Add all to schedule (with release this time)
    release(str1);
    release(str2);
    release(str3);

    CU_ASSERT_EQUAL(lib_linked_list_size(get_schedule_linked_list()), 0);

    // NOTE: no call to free_all() here if your suite or test teardown calls it.
    // Otherwise you can do:
    // free_all();
}

void test_get_and_set_cascade_limit() {
    // Set cascade limit
    set_cascade_limit(2);
    // Check that get_cascade_limit gets the right limit
    CU_ASSERT_EQUAL(get_cascade_limit(), 2);

    // Set new cascade limit
    set_cascade_limit(100);
    // Check that new get_cascade_limit gets the right limit
    CU_ASSERT_EQUAL(get_cascade_limit(), 100);
}

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

node_t *node_create(int val, node_t *left, node_t *right, function1_t destructor) {
    node_t *node = allocate(sizeof(node_t), destructor);
    
    node->val = val;
    node->left = left;
    retain(node->left);
    node->right = right;
    retain(node->right);
    return node;
}

void test_binary_tree_given_destructor_one_node() {
    //Node create retains all pointers
    node_t *n1 = node_create(1, NULL, NULL, node_destroy);
    CU_ASSERT_PTR_NOT_NULL(n1);
    release(n1);
}

void test_binary_tree_default_destructor_one_node() {
    node_t *n1 = node_create(1, NULL, NULL, NULL);
    CU_ASSERT_PTR_NOT_NULL(n1);
    release(n1);
}

void test_binary_tree_given_destructor() {
    set_cascade_limit(10);
    
    // Create a binary tree struct with the library's functions
    /* Tree 1
        4
      2   3
    1
    */
    node_t *n1 = node_create(1, NULL, NULL, node_destroy);
    node_t *n2 = node_create(2, n1, NULL, node_destroy);
    node_t *n3 = node_create(3, NULL, NULL, node_destroy);
    node_t *n4 = node_create(4, n2, n3, node_destroy);

    retain(n4);
    // Create a copy of node 2
    node_t *n2_copy = n2;
    retain(n2);

    // Releases node 4, and with it node 3, but node 2 (and 1) remains since rf > 0
    release(n4);

    // node 2 and 1 should still be reachable
    CU_ASSERT_EQUAL(n2_copy->val, 2);
    CU_ASSERT_EQUAL(n2_copy->left->val, 1);

    CU_ASSERT_EQUAL(rc(n2_copy), 1); 
    release(n2_copy);
    // releasing instead
}

void test_binary_tree_default_destructor() {
    set_cascade_limit(10);
    
    // Create a binary tree struct with the library's functions
    /* Tree 1
        4
      2   3
    1
    */
    node_t *n1 = node_create(1, NULL, NULL, NULL);
    node_t *n2 = node_create(2, n1, NULL, NULL);
    node_t *n3 = node_create(3, NULL, NULL, NULL);
    node_t *n4 = node_create(4, n2, n3, NULL);

    // Create a copy of node 2
    node_t *n2_copy = n2;
    retain(n2);

    // Releases node 4, and with it node 3, but node 2 (and 1) remains since rf > 0
    release(n4);

    // node 2 and 1 should still be reachable
    CU_ASSERT_EQUAL(n2_copy->val, 2);
    CU_ASSERT_EQUAL(n2_copy->left->val, 1);

    CU_ASSERT_EQUAL(rc(n2_copy), 1);
    release(n2_copy);
    // releasing instead
}

void test_large_binary_tree_given_destructor() {
    set_cascade_limit(3);
    
    // Create a binary tree struct with the library's functions
    /* Tree 1
                    11
          4                10
       2     3                9      
    1                       7       8
                        6     5  
    */
    node_t *n1 = node_create(1, NULL, NULL, node_destroy);
    node_t *n2 = node_create(2, n1, NULL, node_destroy);
    node_t *n3 = node_create(3, NULL, NULL, node_destroy);
    node_t *n4 = node_create(4, n2, n3, node_destroy);
    node_t *n5 = node_create(5, NULL, NULL, node_destroy);
    node_t *n6 = node_create(6, NULL, NULL, node_destroy);
    node_t *n7 = node_create(7, n6, n5, node_destroy);
    node_t *n8 = node_create(8, NULL, NULL, node_destroy);
    node_t *n9 = node_create(9, n7, n8, node_destroy);
    node_t *n10 = node_create(10, NULL, n9, node_destroy);
    node_t *n11 = node_create(11, n4, n10, node_destroy);
    retain(n11);

    release(n11);
    CU_ASSERT_EQUAL(lib_linked_list_size(get_schedule_linked_list()), 8);
    cleanup();
    CU_ASSERT_EQUAL(lib_linked_list_size(get_schedule_linked_list()), 0);
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

void weird_array_destroy(obj *object) {
    weird_array_t *arr = (weird_array_t *) object;
    release(arr->i4);
    release(arr->i6);
}

weird_array_t *weird_array_create(char *i4, weird_array_t *i6, function1_t destructor) {
    weird_array_t *arr = allocate(sizeof(weird_array_t), destructor);
    arr->i1 = 1;
    arr->i2 = 2;
    arr->i3 = 3;
    arr->i4 = rc_strdup(i4); 
    retain(arr->i4);
    arr->i5 = 5;
    arr->i6 = i6;
    retain(i6);
    arr->i7 = 7;
    return arr;
}

void test_array_struct_given_destructor() {
    char *str1 = "One";
    weird_array_t *arr1 = weird_array_create(str1, NULL, weird_array_destroy);
    retain(arr1);
    char *str2 = "Two";
    weird_array_t *arr2 = weird_array_create(str2, arr1, weird_array_destroy);
    retain(arr2);
    CU_ASSERT_EQUAL(arr1->i1, arr2->i6->i1);
    CU_ASSERT_EQUAL(strcmp(str1, arr2->i6->i4), 0);

    release(arr1);
    release(arr2);
}

void test_array_struct_default_destructor() {
    char *str1 = "One";
    weird_array_t *arr1 = weird_array_create(str1, NULL, NULL);
    retain(arr1);
    char *str2 = "Two";
    weird_array_t *arr2 = weird_array_create(str2, arr1, NULL);
    retain(arr2);
    CU_ASSERT_EQUAL(arr1->i1, arr2->i6->i1);
    CU_ASSERT_EQUAL(strcmp(str1, arr2->i6->i4), 0);

    release(arr1);
    release(arr2);
}

void test_cleanup(){
    
    set_cascade_limit(1);

    struct cell *c = allocate(sizeof(struct cell), cell_destructor);
    CU_ASSERT_TRUE(rc(c) == 0);
    retain(c);
    CU_ASSERT_TRUE(rc(c) == 1);

    c->cell = allocate(sizeof(struct cell), cell_destructor);
    
    struct cell *c2 = c->cell;
    retain(c2);

    c->cell->cell = allocate(sizeof(struct cell), cell_destructor);
    release(c);

    CU_ASSERT(lib_linked_list_size(get_schedule_linked_list()) == 2);

    cleanup();

    CU_ASSERT(lib_linked_list_size(get_schedule_linked_list()) == 0); 
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
        (CU_add_test(unit_test_suite1, "Get and set cascade limit", test_get_and_set_cascade_limit) == NULL) ||
        (CU_add_test(unit_test_suite1, "test cascade limit", test_cascade_limit) == NULL) ||
        (CU_add_test(unit_test_suite1, "ref count function", test_rc) == NULL) ||
        (CU_add_test(unit_test_suite1, "Allocate and deallocate", test_allocate_and_deallocate) == NULL) ||
        (CU_add_test(unit_test_suite1, "Allocate array and deallocate", test_allocate_array_then_deallocate) == NULL) ||
        (CU_add_test(unit_test_suite1, "1st retain/release test", test_retain_release) == NULL) ||
        (CU_add_test(unit_test_suite1, "2nd retain/release test", test_retain_release2) == NULL) ||
        (CU_add_test(unit_test_suite1, "2nd retain/release test", test_retain_release3) == NULL) ||
        (CU_add_test(unit_test_suite1, "Allocate string and release", test_allocate_strings_then_release) == NULL) ||
        (CU_add_test(unit_test_suite1, "Default destructor", test_default_destructor) == NULL) ||
        (CU_add_test(unit_test_suite1, "Create and destroy a small binary tree with a given destructor", test_binary_tree_given_destructor_one_node) == NULL) ||
        (CU_add_test(unit_test_suite1, "Create and destroy a small binary tree with the default destructor", test_binary_tree_default_destructor_one_node) == NULL) ||
        (CU_add_test(unit_test_suite1, "Create and destroy a binary tree with a given destructor", test_binary_tree_given_destructor) == NULL) ||
        (CU_add_test(unit_test_suite1, "Create and destroy a binary tree with the default destructor", test_binary_tree_default_destructor) == NULL) ||
        (CU_add_test(unit_test_suite1, "Create and destroy a larger binary tree with a given destructor", test_large_binary_tree_given_destructor) == NULL) ||
        (CU_add_test(unit_test_suite1, "Create and destroy a weird array with a given destructor", test_array_struct_given_destructor) == NULL) ||
        (CU_add_test(unit_test_suite1, "Create and destroy a weird array with the default destructor", test_array_struct_default_destructor) == NULL) ||
        (CU_add_test(unit_test_suite1, "Allocate links and release", test_allocate_and_deallocate_links) == NULL) ||
        (CU_add_test(unit_test_suite1, "Cleanup", test_cleanup) == NULL) ||
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