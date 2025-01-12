#include <CUnit/Basic.h>
#include "../src/ref.h"
//files where we apply our ref.h 
#include "../inlupp2/generic_data_structures/common.h"
#include "../inlupp2/generic_data_structures/hash_table.h"
#include "../inlupp2/generic_data_structures/linked_list.h"
#include "../inlupp2/generic_data_structures/iterator.h" 


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

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite ref_suite = CU_add_suite("Unit tests for ref.c when we apply onto inlupp2", init_suite, clean_suite);
    if (ref_suite == NULL) {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        // (CU_add_test(unit_test_suite1, "get_schedule_linked_list test", test_get_schedule_linked_list) == NULL) || //needs to be tested first so that the list is empty
        0
    ) 
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);

    // Run our tests!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
} 