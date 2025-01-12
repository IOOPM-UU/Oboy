#include <CUnit/Basic.h>
#include "common.h"
#include "hash_table.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define Success(v) \
    (option_t) { .success = true, .value = v }
#define Failure() \
    (option_t) { .success = false }
#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)
#define No_Buckets 17

#define str_elem(x) \
    (elem_t) { .p = (x) }

#define int_elem(x) \
    (elem_t) { .i = (x) }

#define ioopm_linked_list_get_int(lst, index, success) \
    ioopm_linked_list_get(lst, index, success).i

#define ioopm_linked_list_get_str(lst, index, success) \
    (char *)ioopm_linked_list_get(lst, index, success).p

#define ioopm_hash_table_create_int_str() \
    ioopm_hash_table_create(key_eq_func, value_eq_func, NULL)

#define ioopm_hash_table_lookup_int(ht, i) \
    ioopm_hash_table_lookup(ht, int_elem(i))

#define ioopm_hash_table_insert_str(ht, key, value) \
    ioopm_hash_table_insert(ht, int_elem(key), str_elem(value))

#define ioopm_hash_table_has_value_str(ht, value) \
    ioopm_hash_table_has_value(ht, str_elem(value))

#define ioopm_hash_table_has_key_int(ht, key) \
    ioopm_hash_table_has_key(ht, int_elem(key))

#define ioopm_hash_table_remove_int(ht, key) \
    ioopm_hash_table_remove(ht, int_elem(key))

bool key_eq_func(elem_t a, elem_t b)
{
    return a.i == b.i;
}

bool value_eq_func(elem_t a, elem_t b)
{
    char *a_ptr = a.p;
    char *b_ptr = b.p;
    return strcmp(a_ptr, b_ptr) == 0;
}

int string_sum_hash(elem_t e)
{
    char *str = e.p;
    int result = 0;
    do
    {
        result += *str;
    } while (*++str != '\0');
    return result;
}

bool key_is_above(elem_t key, elem_t value_ignored, void *n)
{
    int *n_ptr = n;
    return key.i > *n_ptr;
}

void ioopm_entry_printer(elem_t key, elem_t *value, void *extra_ignored)
{
    printf("Key: %d Value:%s\n", key.i, (char *)value->p);
}

void change_value_to_hejsan(elem_t key, elem_t *value, void *extra_ignored)
{
    value->p = "hejsan";
}

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

void test_create_destroy()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}

void test_insert_lookup()
{
    char *d = "d";
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 3, d);
    CU_ASSERT_TRUE(Successful(ioopm_hash_table_lookup_int(ht, 3)));
    CU_ASSERT_STRING_EQUAL((char *)ioopm_hash_table_lookup_int(ht, 3).value.p, d);
    ioopm_hash_table_destroy(ht);
}

void test_remove()
{
    char *d = "d";
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 3, d);
    CU_ASSERT_TRUE(Successful(ioopm_hash_table_remove_int(ht, 3)));
    ioopm_hash_table_destroy(ht);
}

void test_remove_not_inserted()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    CU_ASSERT_TRUE(Unsuccessful(ioopm_hash_table_remove_int(ht, 3)));
    ioopm_hash_table_destroy(ht);
}

void test_remove_does_not_break_chain()
{
    char *d1 = "tia";
    char *d2 = "tjugo7";
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 10, d1);
    ioopm_hash_table_insert_str(ht, 10 + No_Buckets, d2);
    CU_ASSERT_TRUE(Successful(ioopm_hash_table_remove_int(ht, 10)));
    CU_ASSERT_TRUE(Successful(ioopm_hash_table_lookup_int(ht, 10 + No_Buckets)));
    ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    int my_key = 4;
    CU_ASSERT_TRUE(Unsuccessful(ioopm_hash_table_lookup_int(ht, my_key)));
    ioopm_hash_table_insert_str(ht, 4, "value");
    CU_ASSERT_STRING_EQUAL((char *)ioopm_hash_table_lookup_int(ht, my_key).value.p, "value");
    ioopm_hash_table_destroy(ht);
}

void test_insert_duplicate_key()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 3, "value");
    CU_ASSERT_TRUE(Successful(ioopm_hash_table_lookup_int(ht, 3)));
    ioopm_hash_table_insert_str(ht, 3, "value2");
    CU_ASSERT_STRING_EQUAL((char *)ioopm_hash_table_lookup_int(ht, 3).value.p, "value2");
    ioopm_hash_table_destroy(ht);
}

void test_lookup_empty()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    for (int i = 0; i < No_Buckets; ++i)
    {
        CU_ASSERT_TRUE(Unsuccessful(ioopm_hash_table_lookup_int(ht, i)));
    }
    CU_ASSERT_TRUE(Unsuccessful(ioopm_hash_table_lookup_int(ht, -1)));
    ioopm_hash_table_destroy(ht);
}

void test_size_zero()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
    ioopm_hash_table_destroy(ht);
}

void test_size_1()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 3, "d");
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    ioopm_hash_table_destroy(ht);
}

void test_size_4()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 1, "a");
    ioopm_hash_table_insert_str(ht, 2, "b");
    ioopm_hash_table_insert_str(ht, 3, "c");
    ioopm_hash_table_insert_str(ht, 4, "d");
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 4);
    ioopm_hash_table_destroy(ht);
}

void test_is_empty_empty()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
    ioopm_hash_table_destroy(ht);
}

void test_is_empty_not_empty()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 3, "d");
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
    ioopm_hash_table_destroy(ht);
}

void test_ioopm_hash_table_clear()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 1, "a");
    ioopm_hash_table_insert_str(ht, 1 + No_Buckets, "b");
    ioopm_hash_table_insert_str(ht, 10, "c");

    ioopm_hash_table_clear(ht);

    CU_ASSERT_TRUE(Unsuccessful(ioopm_hash_table_lookup_int(ht, 1)));
    CU_ASSERT_TRUE(Unsuccessful(ioopm_hash_table_lookup_int(ht, 1 + No_Buckets)));
    CU_ASSERT_TRUE(Unsuccessful(ioopm_hash_table_lookup_int(ht, 10)));

    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

    ioopm_hash_table_destroy(ht);
}

void test_ioopm_hash_table_keys()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();

    int keys[5] = {3, 10, 42, 0, 99};

    bool found[5] = {false};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert_str(ht, keys[i], "a");
    }

    ioopm_list_t *retrieved_keys = ioopm_hash_table_keys(ht);

    CU_ASSERT_PTR_NOT_NULL(retrieved_keys);

    for (int i = 0; i < 5; i++)
    {
        bool retrieved_success = false;
        for (int j = 0; j < 5; j++)
        {
            if (ioopm_linked_list_get_int(retrieved_keys, i, &retrieved_success) == keys[j])
            {
                found[j] = true;
            }
            if (!retrieved_success && j == 4)
            {
                CU_FAIL("Found a key that was never inserted!");
            }
        }
        retrieved_success = false;
    }

    for (int i = 0; i < 5; i++)
    {
        CU_ASSERT_TRUE(found[i]);
    }

    ioopm_hash_table_destroy(ht);

    ioopm_linked_list_destroy(retrieved_keys);
}

void test_ioopm_hash_table_values()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();

    char *values[5] = {"three", "ten", "fortytwo", "zero", "ninetynine"};

    bool found[5] = {false};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert_str(ht, i, values[i]);
    }

    ioopm_list_t *retrieved_values = ioopm_hash_table_values(ht);

    CU_ASSERT_FATAL(retrieved_values != NULL);

    bool retrieved_success = false;
    for (int i = 0; i < 5; i++)
    {
        bool retrieved_matches_original = false;
        for (int j = 0; j < 5; j++)
        {
            if (strcmp(ioopm_linked_list_get_str(retrieved_values, i, &retrieved_success), values[j]))
            {
                found[j] = true;
                retrieved_matches_original = true;
            }
            if (!retrieved_matches_original && j == 4)
            {
                CU_FAIL("Found a key that was never inserted!");
            }
        }
        retrieved_matches_original = false;
    }

    for (int i = 0; i < 5; i++)
    {
        CU_ASSERT_TRUE(found[i]);
    }

    ioopm_hash_table_destroy(ht);

    ioopm_linked_list_destroy(retrieved_values);
}

void test_ioopm_hash_table_k_v_order()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();

    int keys[5] = {3, 10, 42, 0, 99};

    char *values[5] = {"three", "ten", "fortytwo", "zero", "ninetynine"};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert_str(ht, keys[i], values[i]);
    }

    ioopm_list_t *retrieved_keys = ioopm_hash_table_keys(ht);

    CU_ASSERT_FATAL(retrieved_keys != NULL);

    ioopm_list_t *retrieved_values = ioopm_hash_table_values(ht);

    CU_ASSERT_FATAL(retrieved_values != NULL);

    bool retrieved_matches_original_key;
    bool retrieved_matches_original_value;

    for (int i = 0; i < 5; i++)
    {
        retrieved_matches_original_key = false;
        retrieved_matches_original_value = false;
        bool retrieved_success = false;
        for (int j = 0; j < 5; j++)
        {
            if (ioopm_linked_list_get_int(retrieved_keys, i, &retrieved_success) == keys[j])
            {
                char *get_str = ioopm_linked_list_get_str(retrieved_values, i, &retrieved_success);
                CU_ASSERT_STRING_EQUAL(get_str, values[j]);
                retrieved_matches_original_key = true;
                if (ioopm_linked_list_get_str(retrieved_values, i, &retrieved_success) == values[j])
                {
                    retrieved_matches_original_value = true;
                }
            }
            if (!(retrieved_matches_original_key || retrieved_matches_original_value) &&
                j == 4)
            {
                CU_FAIL("Found a key that was never inserted!");
            }
        }
    }

    ioopm_hash_table_destroy(ht);

    ioopm_linked_list_destroy(retrieved_keys);
    ioopm_linked_list_destroy(retrieved_values);
}

void test_has_key_positive()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 3, "d");
    CU_ASSERT_TRUE(ioopm_hash_table_has_key_int(ht, 3));
    ioopm_hash_table_destroy(ht);
}

void test_has_key_negative()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 3, "d");
    CU_ASSERT_FALSE(ioopm_hash_table_has_key_int(ht, 4));
    ioopm_hash_table_destroy(ht);
}

void test_has_key_empty_ht()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    CU_ASSERT_FALSE(ioopm_hash_table_has_key_int(ht, 3));
    ioopm_hash_table_destroy(ht);
}

void test_has_value_positive()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();

    char *v1 = "hej";

    ioopm_hash_table_insert_str(ht, 3, v1);

    char *copy = strdup(v1);

    CU_ASSERT_TRUE(ioopm_hash_table_has_value_str(ht, v1));    // Test med identiska
    CU_ASSERT_TRUE(ioopm_hash_table_has_value_str(ht, copy));  // Test med ekvivalenta
    CU_ASSERT_TRUE(ioopm_hash_table_has_value_str(ht, "hej")); // Test med ekvivalenta

    ioopm_hash_table_destroy(ht);
    free(copy);
}

void test_has_value_negative()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();

    char *v1 = "hej";

    ioopm_hash_table_insert_str(ht, 10, v1);
    ioopm_hash_table_insert_str(ht, 10, "finns");

    CU_ASSERT_FALSE(ioopm_hash_table_has_value_str(ht, "finns ej"));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_positive()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();

    ioopm_hash_table_insert_str(ht, 11, "a");
    ioopm_hash_table_insert_str(ht, 15, "a");
    ioopm_hash_table_insert_str(ht, 100, "a");

    int n = 10;
    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, key_is_above, &n));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_negative_some()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();

    ioopm_hash_table_insert_str(ht, 11, "a");
    ioopm_hash_table_insert_str(ht, 15, "a");
    ioopm_hash_table_insert_str(ht, 100, "a");

    int n = 30;
    CU_ASSERT_FALSE(ioopm_hash_table_all(ht, key_is_above, &n));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_negative_all()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();

    ioopm_hash_table_insert_str(ht, 0, "a");
    ioopm_hash_table_insert_str(ht, 15, "a");
    ioopm_hash_table_insert_str(ht, 29, "a");

    int n = 30;
    CU_ASSERT_FALSE(ioopm_hash_table_all(ht, key_is_above, &n));
    CU_ASSERT_TRUE(0 < n);
    ioopm_hash_table_destroy(ht);
}

void test_print_and_apply_to_all()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 3, "a");
    ioopm_hash_table_insert_str(ht, 4, "b");
    ioopm_hash_table_insert_str(ht, 5, "c");
    ioopm_hash_table_insert_str(ht, 6, "d");
    ioopm_hash_table_apply_to_all(ht, ioopm_entry_printer, NULL);
    ioopm_hash_table_destroy(ht);
}

void test_apply_to_all_hejsan()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create_int_str();
    ioopm_hash_table_insert_str(ht, 1, "a");
    ioopm_hash_table_insert_str(ht, 2, "b");
    ioopm_hash_table_insert_str(ht, 3, "c");
    ioopm_hash_table_insert_str(ht, 4, "d");
    ioopm_hash_table_apply_to_all(ht, change_value_to_hejsan, NULL);
    for (int i = 0; i < 4; i++)
    {
        CU_ASSERT_STRING_EQUAL((char *)ioopm_hash_table_lookup_int(ht, i + 1).value.p, "hejsan");
    }
    ioopm_hash_table_destroy(ht);
}

void test_lookup_key()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(value_eq_func, value_eq_func, string_sum_hash);

    char *key = strdup("hej");
    char *val = strdup("då");

    ioopm_hash_table_insert(ht, str_elem(key), str_elem(val));

    ioopm_option_t option = ioopm_hash_table_lookup(ht, str_elem("hej"));

    CU_ASSERT_STRING_EQUAL((char *)option.key.p, "hej");
    CU_ASSERT_STRING_EQUAL((char *)option.value.p, "då");

    ioopm_hash_table_destroy(ht);
    free(option.key.p);
    free(option.value.p);
}

void test_remove_key()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(value_eq_func, value_eq_func, string_sum_hash);

    char *key = strdup("hej");
    char *val = strdup("då");

    ioopm_hash_table_insert(ht, str_elem(key), str_elem(val));

    ioopm_option_t option = ioopm_hash_table_remove(ht, str_elem("hej"));

    CU_ASSERT_STRING_EQUAL((char *)option.key.p, "hej");
    CU_ASSERT_STRING_EQUAL((char *)option.value.p, "då");

    ioopm_hash_table_destroy(ht);
    free(option.key.p);
    free(option.value.p);
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
        (CU_add_test(my_test_suite, "A simple test to create and destroy a hashtable", test_create_destroy) == NULL) ||
        (CU_add_test(my_test_suite, "A simple test to insert and look up a value in a hashtable", test_insert_lookup) == NULL) ||
        (CU_add_test(my_test_suite, "A simple test to insert and remove an entry in a hashtable", test_remove) == NULL) ||
        (CU_add_test(my_test_suite, "A test to insert once", test_insert_once) == NULL) ||
        (CU_add_test(my_test_suite, "A test to insert duplicate keys", test_insert_duplicate_key) == NULL) ||
        (CU_add_test(my_test_suite, "A test to lookup in an empty hash table", test_lookup_empty) == NULL) ||
        (CU_add_test(my_test_suite, "A test to remove something that is not in the table", test_remove_not_inserted) == NULL) ||
        (CU_add_test(my_test_suite, "A test to remove something and check that it does not break the linked list", test_remove_does_not_break_chain) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that the size of an empty ht = 0", test_size_zero) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that the size of a ht with 1 mapping is 1", test_size_1) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that the size of a ht with 4 mappings is 4", test_size_4) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that an empty hastable is empty", test_is_empty_empty) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that a non empty hastable is not empty", test_is_empty_not_empty) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that the hash table clears successfully", test_ioopm_hash_table_clear) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that ioopm_hash_table_keys retrives correct keys", test_ioopm_hash_table_keys) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that ioopm_hash_table_values retrives correct values", test_ioopm_hash_table_values) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that keys and values gets stuff in the same order", test_ioopm_hash_table_k_v_order) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that a ht has a key that was inserted", test_has_key_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that a ht does not have key that was not inserted", test_has_key_negative) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that the empty ht does not have key 3", test_has_key_empty_ht) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that has_value() returns true when hash table has value", test_has_value_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that has_value() returns false when hash table does not have value", test_has_value_negative) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that hash_table_all returns true when all entries satisfy predicate", test_hash_table_all_positive) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that hash_table_all returns false when some entries does not satisfy predicate", test_hash_table_all_negative_some) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that hash_table_all returns false when no entry satisfies predicate", test_hash_table_all_negative_all) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that hash_table_apply_to_all with change_value_to_hejsan changes the values to hejsan", test_apply_to_all_hejsan) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that hash_table_lookup returns key", test_lookup_key) == NULL) ||
        (CU_add_test(my_test_suite, "A test to verify that hash_table_remove returns key", test_remove_key) == NULL) ||
        //(CU_add_test(my_test_suite, "A test to verify that apply to all applies to all", test_print_and_apply_to_all) == NULL) || //Denna testar print entry funktionen, men borde kanske lämnas ute för läsbarhet
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