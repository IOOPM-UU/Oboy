#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "../business_logic/backend.h"
#include "../generic_data_structures/common.h"
#include "../generic_data_structures/linked_list.h"
#include "../generic_data_structures/hash_table.h"
#include "../generic_data_structures/iterator.h"
#include "../generic_utils/utils.h"

typedef struct merch ioopm_merch_t;
typedef struct shelf ioopm_shelf_t;
typedef struct shop ioopm_shop_t;
typedef struct shopping_carts shopping_carts_t;
typedef struct cart cart_t;
typedef struct cart_item cart_item_t;

#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)

struct shop
{
    ioopm_hash_table_t *merch_ht;
    ioopm_hash_table_t *locs_ht;
    shopping_carts_t *shopping_carts;
};

struct merch
{
    char *name;
    char *description;
    unsigned int price;
    ioopm_list_t *locs;
};

struct shelf
{
    char *name;
    unsigned int quantity;
};

struct shopping_carts
{
    unsigned int cart_index;
    ioopm_hash_table_t *carts;
};

struct cart
{
    ioopm_list_t *items;
};

struct cart_item
{
    char *name;
    unsigned int quantity;
};

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

void test_create_shop()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    CU_ASSERT_PTR_NOT_NULL(shop);

    ioopm_shop_destroy(shop);
}

void test_add_merch()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    char *name = "apple";
    char *description = "red";
    unsigned int price = 3;
    bool result = ioopm_shop_add_merch(shop, name, description, price);
    CU_ASSERT_TRUE(result);

    ioopm_list_t *merch_list = ioopm_shop_get_merch(shop);

    CU_ASSERT_TRUE(ioopm_linked_list_contains(merch_list, ptr_elem(name)));

    ioopm_linked_list_destroy(merch_list);
    ioopm_shop_destroy(shop);
}

void test_add_already_existing_merch()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    char *name = "apple";
    char *description = "red";
    unsigned int price = 3;
    bool result = ioopm_shop_add_merch(shop, name, description, price);
    CU_ASSERT_TRUE(result);

    result = true;
    result = ioopm_shop_add_merch(shop, name, description, price);
    CU_ASSERT_FALSE(result);

    ioopm_shop_destroy(shop);
}

void test_edit_nonexisting_merch()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    bool success = ioopm_shop_edit_merchandise(shop, "car", "car", "car", 3);
    CU_ASSERT_FALSE(success);
    ioopm_shop_destroy(shop);
}

void test_remove_merch()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    char *name = "apple";
    char *description = "red";
    unsigned int price = 3;
    bool result = ioopm_shop_add_merch(shop, name, description, price);
    CU_ASSERT_TRUE(result);

    bool success = ioopm_shop_remove_merch(shop, name);
    CU_ASSERT_TRUE(success);

    ioopm_shop_destroy(shop);
}

void test_edit_merch_name()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int x = 3;
    ioopm_shop_add_merch(shop, "car", "vroom", x);
    bool success = ioopm_shop_edit_merchandise(shop, "car", "honda", "car", x);
    CU_ASSERT_TRUE(success);

    // Check merch info got updated
    // iop

    // Check merch_ht key got updated
    ioopm_list_t *merch_names = ioopm_shop_get_merch(shop);

    success = false;
    char *merch_name = ioopm_linked_list_get(merch_names, 0, &success).p;
    char *honda = "honda";

    CU_ASSERT_TRUE(success);
    if (success)
    {
        CU_ASSERT_STRING_EQUAL(merch_name, honda);
    }

    ioopm_shop_destroy(shop);
    ioopm_linked_list_destroy(merch_names);
}

void test_edit_merch_and_update_cart_item_name()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int x = 3;
    unsigned int quantity = 5;
    char *old_name = "car";
    char *new_name = "honda";

    ioopm_shop_add_merch(shop, old_name, "vroom", x);
    bool insert_stock = ioopm_shop_insert_stock(shop, old_name, "A35", quantity);
    CU_ASSERT_TRUE(insert_stock);

    unsigned int cart_index = ioopm_shop_create_cart(shop);
    bool add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, old_name, 1);
    CU_ASSERT_TRUE(add_to_cart);

    bool success = ioopm_shop_edit_merchandise(shop, "car", "honda", "car", x);
    CU_ASSERT_TRUE(success);

    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    CU_ASSERT_TRUE(Successful(cart_option));

    ioopm_list_t *items = ((cart_t *)cart_option.value.p)->items;
    cart_item_t tmp = {.name = new_name};
    bool new_name_found = ioopm_linked_list_contains(items, ptr_elem(&tmp));
    CU_ASSERT_TRUE(new_name_found);

    cart_item_t tmp1 = {.name = old_name};
    bool old_name_found = ioopm_linked_list_contains(items, ptr_elem(&tmp1));
    CU_ASSERT_FALSE(old_name_found);

    ioopm_shop_destroy(shop);
}

void test_insert_stock()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int x = 3;

    // adding a merch and inserting a stock to it
    ioopm_shop_add_merch(shop, "car", "vroom", x);
    bool success = ioopm_shop_insert_stock(shop, "car", "A35", 5);
    CU_ASSERT_TRUE(success);

    // looking for the shelf name in the merch locations list
    ioopm_option_t option_merch = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem("car"));
    ioopm_merch_t *merch = option_merch.value.p;
    CU_ASSERT_PTR_NOT_NULL(merch); // Ensure merch is not NULL

    ioopm_list_t *list = merch->locs;
    CU_ASSERT_PTR_NOT_NULL(list);

    // ioopm_shelf_t *shelf = (ioopm_shelf_t *)list->first->next->value.p;
    // char *shelf_name=shelf->name;
    // CU_ASSERT_STRING_EQUAL(shelf_name, "A35");

    // looking for the shelf name in the locations hash table
    ioopm_option_t option_shelf = ioopm_hash_table_lookup(shop->locs_ht, ptr_elem("A35"));
    CU_ASSERT_TRUE(option_shelf.success);
    ioopm_shop_destroy(shop);
}

void test_insert_already_existing_stock()
{

    ioopm_shop_t *shop = ioopm_create_shop();
    // adding a merch with a stock
    ioopm_shop_add_merch(shop, "car", "vroom", 3);
    bool success = ioopm_shop_insert_stock(shop, "car", "A35", 5);
    CU_ASSERT_TRUE(success);

    // adding stock to an already existing one, comparing quantity
    success = ioopm_shop_insert_stock(shop, "car", "A35", 5);
    CU_ASSERT_TRUE(success);

    ioopm_option_t option = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem("car"));
    CU_ASSERT_TRUE(Successful(option));

    ioopm_list_t *locs = ((ioopm_merch_t *)option.value.p)->locs;

    ioopm_shelf_t tmp = (ioopm_shelf_t){.name = "A35"};

    ioopm_list_option_t option_list = ioopm_linked_list_lookup(locs, ptr_elem(&tmp));
    CU_ASSERT_TRUE(Successful(option_list));

    CU_ASSERT_EQUAL(ioopm_linked_list_size(locs), 1);

    CU_ASSERT_TRUE(ioopm_linked_list_contains(locs, ptr_elem(&tmp)));

    unsigned int quantity = ((ioopm_shelf_t *)option_list.value.p)->quantity;
    CU_ASSERT_EQUAL(quantity, 10);

    ioopm_shop_destroy(shop);
}

void test_create_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();

    // skapar 2 carts
    unsigned int index = ioopm_shop_create_cart(shop);
    CU_ASSERT_EQUAL(index, 0);

    index = ioopm_shop_create_cart(shop);
    CU_ASSERT_EQUAL(index, 1);

    ioopm_shop_destroy(shop);
}

void test_remove_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();

    // försöker ta bort en cart som existerar inte
    bool success = ioopm_shop_remove_cart(shop, 2);
    CU_ASSERT_FALSE(success);

    // tar bort en existerande cart
    unsigned int index = ioopm_shop_create_cart(shop);
    success = ioopm_shop_remove_cart(shop, index);
    CU_ASSERT_TRUE(success);

    ioopm_shop_destroy(shop);
}

void test_add_to_nonexisting_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();

    // add to non existent cart
    bool success = ioopm_shop_add_to_cart(shop, 1, "car", 2);
    CU_ASSERT_FALSE(success);

    ioopm_shop_destroy(shop);

    // add more amount that the one reserved for other carts
}

void test_add_to_existing_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);
    char *merch_name = "car";
    char *shelf_name = "A35";
    unsigned int shelf_quantity = 5;
    unsigned int price = 30;
    bool success_add_merch = ioopm_shop_add_merch(shop, merch_name, "red", price);
    CU_ASSERT_TRUE(success_add_merch);

    // lägger till stock i merch
    bool success_insert = ioopm_shop_insert_stock(shop, merch_name, shelf_name, shelf_quantity);
    CU_ASSERT_TRUE(success_insert);

    bool success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, merch_name, 3);
    CU_ASSERT_TRUE(success_add_to_cart);

    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    CU_ASSERT_TRUE(Successful(cart_option));
    if (Successful(cart_option))
    {
        cart_t *cart = cart_option.value.p;
        bool success = false;
        CU_ASSERT_EQUAL(ioopm_linked_list_size(cart->items), 1);
        elem_t item_elem = ioopm_linked_list_get(cart->items, 0, &success);
        if (success)
        {
            cart_item_t *item = item_elem.p;
            CU_ASSERT_EQUAL(item->quantity, 3);
        }
    }
    ioopm_shop_destroy(shop);
}

void test_add_to_cart_more_than_stock()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);
    char *merch_name = "car";
    char *shelf_name = "A35";
    unsigned int shelf_quantity = 5;
    unsigned int price = 30;
    bool success_add_merch = ioopm_shop_add_merch(shop, merch_name, "red", price);
    CU_ASSERT_TRUE(success_add_merch);

    // lägger till stock i merch
    bool success_insert = ioopm_shop_insert_stock(shop, merch_name, shelf_name, shelf_quantity);
    CU_ASSERT_TRUE(success_insert);

    bool success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, merch_name, 3);
    CU_ASSERT_TRUE(success_add_to_cart);

    // Add to much
    success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, merch_name, 5);
    CU_ASSERT_FALSE(success_add_to_cart);

    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    CU_ASSERT_TRUE(Successful(cart_option));
    if (Successful(cart_option))
    {
        cart_t *cart = cart_option.value.p;
        bool success = false;
        CU_ASSERT_EQUAL(ioopm_linked_list_size(cart->items), 1);
        elem_t item_elem = ioopm_linked_list_get(cart->items, 0, &success);
        if (success)
        {
            cart_item_t *item = item_elem.p;
            CU_ASSERT_EQUAL(item->quantity, 3);
        }
    }

    ioopm_shop_destroy(shop);
}

void test_add_to_cart_more_than_carts()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);
    unsigned int cart_index2 = ioopm_shop_create_cart(shop);
    char *merch_name = "car";
    char *shelf_name = "A35";
    unsigned int shelf_quantity = 5;
    unsigned int price = 30;
    bool success_add_merch = ioopm_shop_add_merch(shop, merch_name, "red", price);
    CU_ASSERT_TRUE(success_add_merch);

    // lägger till stock i merch
    bool success_insert = ioopm_shop_insert_stock(shop, merch_name, shelf_name, shelf_quantity);
    CU_ASSERT_TRUE(success_insert);

    // lägger till en 3 * merch i cart
    bool success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, merch_name, 3);
    CU_ASSERT_TRUE(success_add_to_cart);

    // Add to much
    success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index2, merch_name, 3);
    CU_ASSERT_FALSE(success_add_to_cart);

    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    CU_ASSERT_TRUE(Successful(cart_option));
    if (Successful(cart_option))
    {
        cart_t *cart = cart_option.value.p;
        bool success = false;
        CU_ASSERT_EQUAL(ioopm_linked_list_size(cart->items), 1);
        elem_t item_elem = ioopm_linked_list_get(cart->items, 0, &success);
        if (success)
        {
            cart_item_t *item = item_elem.p;
            CU_ASSERT_EQUAL(item->quantity, 3);
        }
    }

    ioopm_shop_destroy(shop);
}

void test_add_to_cart_all_stock()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);
    unsigned int cart_index2 = ioopm_shop_create_cart(shop);
    char *merch_name = "car";
    char *shelf_name = "A35";
    unsigned int shelf_quantity = 5;
    unsigned int price = 30;
    bool success_add_merch = ioopm_shop_add_merch(shop, merch_name, "red", price);
    CU_ASSERT_TRUE(success_add_merch);

    // lägger till stock i merch
    bool success_insert = ioopm_shop_insert_stock(shop, merch_name, shelf_name, shelf_quantity);
    CU_ASSERT_TRUE(success_insert);

    // lägger till en 3 * merch i cart
    bool success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, merch_name, 3);
    CU_ASSERT_TRUE(success_add_to_cart);

    success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index2, merch_name, 2);
    CU_ASSERT_TRUE(success_add_to_cart);

    // Add to much
    success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index2, merch_name, 1);
    CU_ASSERT_FALSE(success_add_to_cart);

    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    CU_ASSERT_TRUE(Successful(cart_option));
    if (Successful(cart_option))
    {
        cart_t *cart = cart_option.value.p;
        bool success = false;
        CU_ASSERT_EQUAL(ioopm_linked_list_size(cart->items), 1);
        elem_t item_elem = ioopm_linked_list_get(cart->items, 0, &success);
        if (success)
        {
            cart_item_t *item = item_elem.p;
            CU_ASSERT_EQUAL(item->quantity, 3);
        }

        CU_ASSERT_EQUAL(ioopm_linked_list_size(cart->items), 1);
        elem_t item_elem2 = ioopm_linked_list_get(cart->items, 1, &success);
        if (success)
        {
            cart_item_t *item2 = item_elem2.p;
            CU_ASSERT_EQUAL(item2->quantity, 2);
        }
    }

    ioopm_shop_destroy(shop);
}

void test_add_to_cart_zero()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);
    char *merch_name = "car";
    char *shelf_name = "A35";
    unsigned int shelf_quantity = 5;
    unsigned int price = 30;
    bool success_add_merch = ioopm_shop_add_merch(shop, merch_name, "red", price);
    CU_ASSERT_TRUE(success_add_merch);

    // lägger till stock i merch
    bool success_insert = ioopm_shop_insert_stock(shop, merch_name, shelf_name, shelf_quantity);
    CU_ASSERT_TRUE(success_insert);

    // lägger till en 3 * merch i cart
    bool success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, merch_name, 0);
    CU_ASSERT_FALSE(success_add_to_cart);

    ioopm_shop_destroy(shop);
}

void test_remove_from_empty_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    // lägger till en cart med inga items
    unsigned int cart_index = ioopm_shop_create_cart(shop);
    bool success = ioopm_shop_remove_from_cart(shop, cart_index, "car", 3);
    CU_ASSERT_FALSE(success);
    ioopm_shop_destroy(shop);
}

void test_remove_from_non_existent_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();

    bool success = ioopm_shop_remove_from_cart(shop, 0, "car", 3);
    CU_ASSERT_FALSE(success);
    ioopm_shop_destroy(shop);
}

void test_remove_cart_item_totally_from_existing_item_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);
    char *merch_name = "car";
    char *shelf_name = "A35";
    unsigned int shelf_quantity = 5;
    unsigned int price = 30;
    bool success_add_merch = ioopm_shop_add_merch(shop, merch_name, "red", price);
    CU_ASSERT_TRUE(success_add_merch);

    // lägger till stock i merch
    bool success_insert = ioopm_shop_insert_stock(shop, merch_name, shelf_name, shelf_quantity);
    CU_ASSERT_TRUE(success_insert);

    // lägger till all merch från shelf i cart
    bool success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, merch_name, shelf_quantity);
    CU_ASSERT_TRUE(success_add_to_cart);

    // tar bort alla
    bool success_remove = ioopm_shop_remove_from_cart(shop, cart_index, merch_name, shelf_quantity);
    CU_ASSERT_TRUE(success_remove);

    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    CU_ASSERT_FATAL(Successful(cart_option));

    cart_t *cart = cart_option.value.p;
    bool success = false;
    CU_ASSERT_EQUAL(ioopm_linked_list_size(cart->items), 0);
    elem_t item_elem = ioopm_linked_list_get(cart->items, 0, &success);
    CU_ASSERT_FALSE(success);
    CU_ASSERT_PTR_NULL(item_elem.p);

    ioopm_shop_destroy(shop);
}

void test_calculate_cost_empty_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);
    unsigned int cost = ioopm_shop_calculate_cost(shop, cart_index);

    CU_ASSERT_EQUAL(cost, 0);
    ioopm_shop_destroy(shop);
}

void test_calculate_cost_non_existing_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();

    unsigned int cost = ioopm_shop_calculate_cost(shop, 5);
    CU_ASSERT_EQUAL(cost, 0);

    ioopm_shop_destroy(shop);
}

void test_calculate_cost_multiple_items_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);

    // add merch to shop
    bool success_add_merch1 = ioopm_shop_add_merch(shop, "car", "red", 30);
    bool success_add_merch2 = ioopm_shop_add_merch(shop, "bike", "blue", 20);
    CU_ASSERT_TRUE(success_add_merch1);
    CU_ASSERT_TRUE(success_add_merch2);

    // add stock to the merch
    bool success_insert1 = ioopm_shop_insert_stock(shop, "car", "A35", 10);
    bool success_insert2 = ioopm_shop_insert_stock(shop, "bike", "B12", 5);
    CU_ASSERT_TRUE(success_insert1);
    CU_ASSERT_TRUE(success_insert2);

    // add items to the cart
    bool success_add_to_cart1 = ioopm_shop_add_to_cart(shop, cart_index, "car", 3);  // 3 * 30 = 90
    bool success_add_to_cart2 = ioopm_shop_add_to_cart(shop, cart_index, "bike", 2); // 2 * 20 = 40
    CU_ASSERT_TRUE(success_add_to_cart1);
    CU_ASSERT_TRUE(success_add_to_cart2);

    // calculate the total cost of the cart (90 + 40 = 130)
    unsigned int cost = ioopm_shop_calculate_cost(shop, cart_index);
    CU_ASSERT_EQUAL(cost, 130);

    ioopm_shop_destroy(shop);
}

void test_calculate_cost_single_item_cart()
{
    ioopm_shop_t *shop = ioopm_create_shop();
    unsigned int cart_index = ioopm_shop_create_cart(shop);

    bool success_add_merch = ioopm_shop_add_merch(shop, "car", "red", 50);
    CU_ASSERT_TRUE(success_add_merch);

    bool success_insert = ioopm_shop_insert_stock(shop, "car", "A35", 10);
    CU_ASSERT_TRUE(success_insert);

    // add a single item to the cart
    bool success_add_to_cart = ioopm_shop_add_to_cart(shop, cart_index, "car", 1); // 1 * 50 = 50
    CU_ASSERT_TRUE(success_add_to_cart);

    // calculate the total cost of the cart (50)
    unsigned int cost = ioopm_shop_calculate_cost(shop, cart_index);
    CU_ASSERT_EQUAL(cost, 50);

    ioopm_shop_destroy(shop);
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

        (CU_add_test(my_test_suite, "test process word insert new", test_create_shop) == NULL) ||
        (CU_add_test(my_test_suite, "test add merch", test_add_merch) == NULL) ||
        (CU_add_test(my_test_suite, "test add already existing merch", test_add_already_existing_merch) == NULL) ||
        (CU_add_test(my_test_suite, "test edit a non existing merch", test_edit_nonexisting_merch) == NULL) ||
        (CU_add_test(my_test_suite, "test remove merch", test_remove_merch) == NULL) ||
        (CU_add_test(my_test_suite, "test edit merch name", test_edit_merch_name) == NULL) ||
        (CU_add_test(my_test_suite, "test edit merch and update cart item name", test_edit_merch_and_update_cart_item_name) == NULL) ||
        (CU_add_test(my_test_suite, "test insert stock", test_insert_stock) == NULL) ||
        (CU_add_test(my_test_suite, "test insert already existing stock", test_insert_already_existing_stock) == NULL) ||
        (CU_add_test(my_test_suite, "test create cart", test_create_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test remove cart", test_remove_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test add to non existing cart", test_add_to_nonexisting_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test add already existing cart", test_add_already_existing_merch) == NULL) ||
        (CU_add_test(my_test_suite, "test add cart item to existing cart", test_add_to_existing_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test add to cart more than stock available ", test_add_to_cart_more_than_stock) == NULL) ||
        (CU_add_test(my_test_suite, "test add to cart all stock ", test_add_to_cart_all_stock) == NULL) ||
        (CU_add_test(my_test_suite, "test add to cart zero items ", test_add_to_cart_zero) == NULL) ||
        (CU_add_test(my_test_suite, "test remove from empty ", test_remove_from_empty_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test remove from non existent cart ", test_remove_from_non_existent_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test remove cart item totally from existing item ", test_remove_cart_item_totally_from_existing_item_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test calculate cost empty cart", test_calculate_cost_empty_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test calculate cost non existing cart", test_calculate_cost_non_existing_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test calculate cost multiple items cart", test_calculate_cost_multiple_items_cart) == NULL) ||
        (CU_add_test(my_test_suite, "test calculate cost single item cart", test_calculate_cost_single_item_cart) == NULL) ||

        0)
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
