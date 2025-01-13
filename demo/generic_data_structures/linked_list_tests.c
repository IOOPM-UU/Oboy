#include <CUnit/Basic.h>
#include "linked_list.h"
#include <stdbool.h>

#define int_elem(x) \
    (elem_t) { .i = (x) }

#define ioopm_linked_list_append_int(lst, i) \
    ioopm_linked_list_append(lst, int_elem(i))

#define ioopm_linked_list_prepend_int(lst, i) \
    ioopm_linked_list_prepend(lst, int_elem(i))

#define ioopm_linked_list_get_int(lst, index, success) \
    ioopm_linked_list_get(lst, index, success).i

#define ioopm_linked_list_contains_int(lst, i) \
    ioopm_linked_list_contains(lst, int_elem(i))

#define ioopm_linked_list_insert_int(lst, i, v) \
    ioopm_linked_list_insert(lst, i, int_elem(v))

#define ioopm_linked_list_remove_int(lst, index, s) \
    ioopm_linked_list_remove(lst, index, s).i

#define ioopm_linked_list_create_int() \
    ioopm_linked_list_create(int_eq_func)



/* Helper functions */

bool int_eq_func(elem_t a, elem_t b)
{
    return a.i == b.i;
}

bool element_is_less_than(int index, elem_t value, void *v)
{
    int *v1 = v;
    return value.i < *v1;
}

void change_value_with_n(int index, elem_t *value, void *n)
{
    int *n_ptr = n;
    value->i = *n_ptr;
}

static void ioopm_increase_value_by_extra(int index_ignored, elem_t *value, void *extra)
{
    int *extra_ptr = extra;
    value->i += *extra_ptr;
}

/**/

int init_suite(void)
{
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void)
{

    // Change this function if you want to do something *after* you
    // run a test suite
    return 0;
}

// These are example test functions. You should replace them with
// functions of your own.
void test_linked_list_append()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 10);
    ioopm_linked_list_append_int(lst, 20);
    CU_ASSERT_TRUE(ioopm_linked_list_size(lst) == 2);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_prepend()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_prepend_int(lst, 10);
    ioopm_linked_list_prepend_int(lst, 20);
    CU_ASSERT_TRUE(ioopm_linked_list_size(lst) == 2);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_append_and_prepend()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 10);
    ioopm_linked_list_prepend_int(lst, 20);
    CU_ASSERT_TRUE(ioopm_linked_list_size(lst) == 2);
    ioopm_linked_list_destroy(lst);
}

void test_create_empty_list()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    CU_ASSERT_PTR_NOT_NULL(lst);
    ioopm_linked_list_destroy(lst);
}

void test_append_positive_1()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 3);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 3);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_append_positive_2()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 1, &success), 4);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_append_negative_1()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 3);
    bool success = false;
    CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 4);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_append_negative_2()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 3);
    CU_ASSERT_TRUE(success);
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 1, &success), 4);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_prepend_positive_1()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_prepend_int(lst, 3);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 3);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_prepend_positive_2()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_prepend_int(lst, 3);
    ioopm_linked_list_prepend_int(lst, 5);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 5);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_prepend_negative_1()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_prepend_int(lst, 3);
    bool success = false;
    CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 4);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_prepend_negative_2()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_prepend_int(lst, 3);
    ioopm_linked_list_prepend_int(lst, 4);
    bool success = false;
    CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 3);
    CU_ASSERT_TRUE(success);
    CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get_int(lst, 1, &success), 4);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_insert_positive()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_insert_int(lst, 0, 1);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 1);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_insert_negative()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_insert_int(lst, 0, 1);
    bool success = false;
    CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 4);
    CU_ASSERT_TRUE(success);

    ioopm_linked_list_destroy(lst);
}

void test_linked_list_insert_ne_list()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 4);
    ioopm_linked_list_append_int(lst, 5);
    ioopm_linked_list_append_int(lst, 4);
    ioopm_linked_list_append_int(lst, 4);
    ioopm_linked_list_append_int(lst, 4);
    ioopm_linked_list_insert_int(lst, 0, 1);
    ioopm_linked_list_insert_int(lst, 2, 10); 
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 1);
    CU_ASSERT_TRUE(success);
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 2, &success), 10);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_remove_return_value()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_insert_int(lst, 0, 4);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_remove_int(lst, 0, &success), 4);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_remove()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    bool success = false;
    int w; // To avoid compile warnings
    w = ioopm_linked_list_remove_int(lst, 0, &success);
    CU_ASSERT_EQUAL(ioopm_linked_list_remove_int(lst, 0, &success), 4);
    CU_ASSERT_TRUE(success);

    w = 0;
    CU_ASSERT_EQUAL(w, 0); // To avoid compile warnings

    ioopm_linked_list_destroy(lst);
}

void test_linked_list_remove_list_status() 
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_insert_int(lst, 0, 4);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 4);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_get_empty_list()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    bool success = false;
    int i = ioopm_linked_list_get_int(lst, 0, &success);
    CU_ASSERT_EQUAL(i, 0);
    CU_ASSERT_FALSE(success);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lst));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 0);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_get_positive()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 3);
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, 0, &success), 3);
    CU_ASSERT_TRUE(success);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_get_negative()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 3);
    bool success = false;
    CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get_int(lst, 2, &success), 2);
    CU_ASSERT_FALSE(success);
    ioopm_linked_list_destroy(lst);
}

void test_linked_empty_list_contains()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    CU_ASSERT_FALSE(ioopm_linked_list_contains_int(lst, 4));
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_contains_negative()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 3);
    CU_ASSERT_FALSE(ioopm_linked_list_contains_int(lst, 4));
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_contains_positive()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 3);
    CU_ASSERT_TRUE(ioopm_linked_list_contains_int(lst, 3));
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_size_empty_list()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 0);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_size_ne_list()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 1);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 4);
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_is_empty_positive()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lst));
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_is_empty_negative()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 4);
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(lst));
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_all_positive()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 1);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);
    ioopm_linked_list_append_int(lst, 5);
    int n = 10;
    CU_ASSERT_TRUE(ioopm_linked_list_all(lst, element_is_less_than, &n));
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_all_negative() 
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 1);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);
    ioopm_linked_list_append_int(lst, 5);
    int n = 1;
    CU_ASSERT_FALSE(ioopm_linked_list_all(lst, element_is_less_than, &n));
    ioopm_linked_list_destroy(lst);
}

void test_linked_list_all_empty()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();

    int n = 1;

    CU_ASSERT_FALSE(ioopm_linked_list_all(lst, element_is_less_than, &n));

    ioopm_linked_list_destroy(lst);
}

void test_linked_list_all_edge() 
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 0);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);
    ioopm_linked_list_append_int(lst, 5);
    int n = 1;
    CU_ASSERT_FALSE(ioopm_linked_list_all(lst, element_is_less_than, &n));
    ioopm_linked_list_destroy(lst);
}

void test_ioopm_linked_list_apply_to_all()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();

    ioopm_linked_list_append_int(lst, 1);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);

    int n = 10;
    ioopm_linked_list_apply_to_all(lst, change_value_with_n, &n);
    bool success = false;
    for (int i = 0; i < 4; i++)
    {
        CU_ASSERT_EQUAL(ioopm_linked_list_get_int(lst, i, &success), 10);
        CU_ASSERT_TRUE(success);
    }

    ioopm_linked_list_destroy(lst);
}

void test_ioopm_linked_list_apply_to_all_2()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();

    ioopm_linked_list_append_int(lst, 1);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);

    int n = 10;
    ioopm_linked_list_apply_to_all(lst, ioopm_increase_value_by_extra, &n);
    bool success = false;
    for (int i = 0; i < 4; i++)
    {
        CU_ASSERT_TRUE(ioopm_linked_list_get_int(lst, i, &success) > 10);
        CU_ASSERT_TRUE(success);
    }

    ioopm_linked_list_destroy(lst);
}

void test_any_positive()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();

    ioopm_linked_list_append_int(lst, 1);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);

    int n = 10;

    CU_ASSERT_TRUE(ioopm_linked_list_any(lst, element_is_less_than, &n));

    ioopm_linked_list_destroy(lst);
}

void test_any_negative()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();

    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 3);
    ioopm_linked_list_append_int(lst, 4);
    ioopm_linked_list_append_int(lst, 5);

    int n = 1;

    CU_ASSERT_FALSE(ioopm_linked_list_any(lst, element_is_less_than, &n));

    ioopm_linked_list_destroy(lst);
}

void test_any_empty()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();

    int n = 100;

    CU_ASSERT_FALSE(ioopm_linked_list_any(lst, element_is_less_than, &n));

    ioopm_linked_list_destroy(lst);
}

void test_clear()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_append_int(lst, 2);
    ioopm_linked_list_clear(lst);

    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lst));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 0);

    ioopm_linked_list_destroy(lst);
}

// void test_append_does_not_break_chain()
// {
//     ioopm_list_t *lst = ioopm_linked_list_create_int();
//     ioopm_linked_list_append_int(lst, 1);
//     ioopm_linked_list_append_int(lst, 2);

//     CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 2);

//     CU_ASSERT_TRUE(lst->first->next->next == lst->last);
//     CU_ASSERT_TRUE(lst->first->next == lst->last->previous);
//     CU_ASSERT_TRUE(lst->first == lst->last->previous->previous);

//     ioopm_linked_list_destroy(lst);
// }

// void test_prepend_does_not_break_chain()
// {
//     ioopm_list_t *lst = ioopm_linked_list_create_int();
//     ioopm_linked_list_prepend_int(lst, 2);
//     ioopm_linked_list_prepend_int(lst, 1);

//     CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 2);

//     CU_ASSERT_TRUE(lst->first->next->next == lst->last);
//     CU_ASSERT_TRUE(lst->first->next == lst->last->previous);
//     CU_ASSERT_TRUE(lst->first == lst->last->previous->previous);

//     ioopm_linked_list_destroy(lst);
// }

void test_linked_list_remove_on_empty_list()
{
    ioopm_list_t *lst = ioopm_linked_list_create_int();
    bool success = false;
    int w; // To avoid compile warnings
    w = ioopm_linked_list_remove_int(lst, 0, &success);
    CU_ASSERT_FALSE(success);

    w = 0;
    CU_ASSERT_EQUAL(w, 0); // To avoid compile warnings
    
    ioopm_linked_list_destroy(lst);
}

int main()
{
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
    if (my_test_suite == NULL)
    {
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
        (CU_add_test(my_test_suite, "A test to check that append works", test_linked_list_append) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that prepend works", test_linked_list_prepend) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that append and prepend works together", test_linked_list_append_and_prepend) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that the pointer to an empty list doesn`t point to NULL", test_create_empty_list) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that append on an empty list makes index 0 the appended element", test_append_positive_1) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that appending two items to an empty list makes index one the last one appended", test_append_positive_2) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that appending 3 to an empty list does not make the value 4 at index 0", test_append_negative_1) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that appending 3 and then 4 to an empty list does not make index 1 3", test_append_negative_2) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that prepending 3 to and empty list makes index 0 3", test_prepend_positive_1) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that prepending 3 then 5 to and empty list makes index 1 5", test_prepend_positive_2) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that prepending 3 to an empty list does not make the 0th index 4", test_prepend_negative_1) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that prepending 3 and then 4 to an empty list does not make the first entry 3", test_prepend_negative_2) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that inserting a 1 at index 0 really inserts a 1 at index 0", test_linked_list_insert_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that inserting a 1 at index 0 doesnt insert a 4 at index 0", test_linked_list_insert_negative) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that inserting a 1 at index 0 in a non-empty list inserts 1 at index 0", test_linked_list_insert_ne_list) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that remove returns the integer it removed", test_linked_list_remove_return_value) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that empty list at index one is a null ptr, is_empty on empty list is true, and that the size is equal to 0", test_linked_list_get_empty_list) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that get correctly gets the 0th element, and that the element is correct", test_linked_list_get_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that get doesnt get something else", test_linked_list_get_negative) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that the empty list doesnt contain 4", test_linked_empty_list_contains) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that the a non empty list without 4 doesnt contain 4", test_linked_list_contains_negative) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that a list with just 3 contains a list with just 3", test_linked_list_contains_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that the size of an empty list is 0", test_linked_list_size_empty_list) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that the size of a list with 4 elements is 4", test_linked_list_size_ne_list) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that an empty list is empty", test_linked_list_is_empty_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that a non empty list is not emmpty", test_linked_list_is_empty_negative) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that all elements in a list with elements less than 10 are indeed less than 10", test_linked_list_all_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that all elements in a list with elements greater than 1 are less than 1", test_linked_list_all_negative) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that all elements in a list with one 0 element", test_linked_list_all_edge) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that apply to all with change value to n changes every value to n", test_ioopm_linked_list_apply_to_all) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that apply to all with increase value with n changes every value with n", test_ioopm_linked_list_apply_to_all_2) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that any element is less than 10 in a list of elements less than 10", test_any_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that any element is less than 1 in a list of elements greater than 1", test_any_negative) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that any element in an empty list is less than 100", test_any_empty) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that clear clears list", test_clear) == NULL) ||
        // (CU_add_test(my_test_suite, "A test to check that append does not break chain", test_append_does_not_break_chain) == NULL) ||
        // (CU_add_test(my_test_suite, "A test to check that prepend does not break chain", test_prepend_does_not_break_chain) == NULL) ||
        (CU_add_test(my_test_suite, "A test to check that remove on an empty list fails", test_linked_list_remove_on_empty_list) == NULL) ||
        0)
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_NORMAL);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
}