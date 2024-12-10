#include "../src/ref.h"
#include <CUnit/Basic.h>
#include <stdlib.h>

int init_suite(void) {
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void) {
    // Change this function if you want to do something *after* you
    // run a test suite
    return 0;
}

// Unit tests

// deallocate

// allocate
void test_alloc(void) {
    obj_t *obj = NULL;
    CU_ASSERT_PTR_NULL(obj);
    obj = allocate(sizeof(obj));
    CU_ASSERT_PTR_NOT_NULL(obj);
    deallocate(obj);
    CU_ASSERT_PTR_NULL(obj);
}

void test2(void) {
    CU_ASSERT_EQUAL(1 + 1, 2);
}

void test_is_number(void) {
    CU_ASSERT_TRUE(is_number("42"));
    CU_ASSERT_TRUE(is_number("-10"));
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
        (CU_add_test(unit_test_suite1, "A simple test", test1) == NULL) ||
        (CU_add_test(unit_test_suite1, "Basic arithmetics", test2) == NULL) ||
        (CU_add_test(unit_test_suite1, "Basic tests of is_number", test_is_number) == NULL) || 0
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