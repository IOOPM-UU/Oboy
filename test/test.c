#include "../src/ref.h"
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <math.h>

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
void dummy_destructor(voidptr) {
        free(voidptr);
    }

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
}
void dummy_destructor(void *ptr) {
        free(ptr);
    }

void test_memdata_generate(void) {
    memdata_t *memdata = memdata_generate(dummy_destructor);
        CU_ASSERT_PTR_NOT_NULL(memdata);
        CU_ASSERT_EQUAL(memdata->rc, 0);
        CU_ASSERT_EQUAL(memdata->destructor, dummy_destructor);
        memdata->destructor(memdata); 
}
// currently leaks, need a special destructor for memdata
void test_memdata_generate_insert_ht(void) {
    memdata_t *memdata = memdata_generate(dummy_destructor);
    CU_ASSERT_PTR_NOT_NULL(memdata);
    ioopm_hash_table_insert(get_memdata_ht(), int_elem(1), ptr_elem(memdata_generate(dummy_destructor)));
    ioopm_option_t option =  ioopm_hash_table_lookup(get_memdata_ht(), int_elem(1));
    CU_ASSERT_TRUE(option.success);
    ioopm_hash_table_insert(get_memdata_ht(), int_elem(2), ptr_elem(memdata_generate(dummy_destructor)));
    option = ioopm_hash_table_lookup(get_memdata_ht(), int_elem(2));
    CU_ASSERT_TRUE(option.success);
    option = ioopm_hash_table_lookup(get_memdata_ht(), int_elem(1));
    CU_ASSERT_TRUE(option.success);
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
        // (CU_add_test(unit_test_suite1, "A simple test", test1) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Basic arithmetics", test2) == NULL) ||
        // (CU_add_test(unit_test_suite1, "Basic tests of is_number", test_is_number) == NULL)
        (CU_add_test(unit_test_suite1, "Get memdata", test_get_memdata_ht) == NULL) ||
        (CU_add_test(unit_test_suite1, "Get memdata", test_get_memdata_ht_retrieve) == NULL) ||
        (CU_add_test(unit_test_suite1, "memdata generate", test_memdata_generate) == NULL) ||
         (CU_add_test(unit_test_suite1, "memdata generate_insert", test_memdata_generate_insert_ht) == NULL) ||
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