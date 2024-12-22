#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "backend.h"
#include "../generic_data_structures/common.h"
#include "../generic_data_structures/linked_list.h"
#include "../generic_data_structures/hash_table.h"
#include "../generic_data_structures/iterator.h"
#include "../generic_utils/utils.h"

#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)

bool int_eq(elem_t e1, elem_t e2)
{
    return e1.i == e2.i;
}

bool cart_item_eq(elem_t e1, elem_t e2)
{
    cart_item_t *item1 = e1.p;
    cart_item_t *item2 = e2.p;

    return strcmp(item1->name, item2->name) == 0;
}

static unsigned int quantity_in_one_cart(cart_t *cart, ioopm_merch_t *merch)
{
    if (!cart)
    {
        return 0;
    }

    ioopm_list_iterator_t *iter = ioopm_list_iterator(cart->items);

    unsigned int quantity_in_one_cart = 0;

    bool success = true;
    while (success)
    {
        cart_item_t *cart_item = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            if (strcmp(merch->name, cart_item->name) == 0)
            {
                quantity_in_one_cart += cart_item->quantity;
            }
        }
        ioopm_iterator_next(iter, &success);
    }
    ioopm_iterator_destroy(iter);
    return quantity_in_one_cart;
}

static unsigned int quantity_in_carts(ioopm_shop_t *shop, ioopm_merch_t *merch)
{
    ioopm_list_t *shopping_carts = ioopm_hash_table_values(shop->shopping_carts->carts);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(shopping_carts);

    unsigned int quantity_in_carts = 0;

    bool success = true;
    while (success)
    {
        cart_t *cart = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            quantity_in_carts += quantity_in_one_cart(cart, merch);
        }
        ioopm_iterator_next(iter, &success);
    }

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(shopping_carts);
    return quantity_in_carts;
}

static unsigned int quantity_in_stock(ioopm_shop_t *shop, ioopm_merch_t *merch)
{
    ioopm_list_t *quantity_list = ioopm_shop_get_stock(shop, merch->name);
    if (quantity_list == NULL)
    {
        return 0;
    }

    ioopm_list_iterator_t *iter = ioopm_list_iterator(quantity_list);

    unsigned int quantity_in_stock = 0;

    bool success = true;
    while (success)
    {
        ioopm_shelf_t *shelf = ioopm_iterator_current(iter, &success).p;

        if (success)
        {
            quantity_in_stock += shelf->quantity;
        }

        ioopm_iterator_next(iter, &success);
    }
    ioopm_linked_list_destroy(quantity_list);
    ioopm_iterator_destroy(iter);
    return quantity_in_stock;
}

shopping_carts_t *create_shopping_cart()
{
    shopping_carts_t *shopping_carts = calloc(1, sizeof(shopping_carts_t));
    assert(!pointer_is_null(shopping_carts));
    shopping_carts->cart_index = 0;
    shopping_carts->carts = ioopm_hash_table_create(int_eq, cart_item_eq, NULL);
    return shopping_carts;
}

void destroy_cart(cart_t *cart)
{
    ioopm_list_iterator_t *iter = ioopm_list_iterator(cart->items);

    bool success = true;
    while (ioopm_iterator_has_next(iter) || success)
    {
        cart_item_t *cart_item = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            destroy_cart_item(cart_item);
        }
        ioopm_iterator_next(iter, &success);
    }

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(cart->items);
    free(cart);
}

void destroy_shopping_cart(shopping_carts_t *shopping_carts)
{
    ioopm_list_t *carts = ioopm_hash_table_values(shopping_carts->carts);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(carts);

    ioopm_hash_table_destroy(shopping_carts->carts);

    bool success = true;
    while (success)
    {
        cart_t *cart = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            destroy_cart(cart);
        }
        ioopm_iterator_next(iter, &success);
    }

    ioopm_linked_list_destroy(carts);
    ioopm_iterator_destroy(iter);
    free(shopping_carts);
}

unsigned int ioopm_shop_create_cart(ioopm_shop_t *shop)
{
    unsigned int index = shop->shopping_carts->cart_index;

    cart_t *cart = calloc(1, sizeof(cart_t));
    assert(!pointer_is_null(cart));

    cart->items = ioopm_linked_list_create(cart_item_eq);
    ioopm_hash_table_insert(shop->shopping_carts->carts, u_elem(index), ptr_elem(cart));
    shop->shopping_carts->cart_index++;

    return index;
}

bool ioopm_shop_remove_cart(ioopm_shop_t *shop, unsigned int index)
{
    ioopm_option_t success = ioopm_hash_table_remove(shop->shopping_carts->carts, u_elem(index));
    if (Unsuccessful(success))
    {
        return false;
    }

    cart_t *cart = success.value.p;
    destroy_cart(cart);
    return Successful(success);
}

cart_item_t *create_cart_item(char *name, unsigned int quantity)
{
    cart_item_t *cart = calloc(1, sizeof(cart_item_t));
    assert(!pointer_is_null(cart));

    cart->name = strdup(name);
    cart->quantity = quantity;
    return cart;
}

bool ioopm_shop_add_to_cart(ioopm_shop_t *shop, unsigned int cart_index, char *merch_name, unsigned int amount)
{
    if (amount == 0)
    {
        return false;
    }

    ioopm_option_t option = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem(merch_name));
    if (Unsuccessful(option))
    {
        return false; // om merchen ej existerar
    }

    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    if (Unsuccessful(cart_option))
    {
        return false; // om cart ej existerar
    }

    ioopm_merch_t *merch = option.value.p;
    unsigned int quantity_stock = quantity_in_stock(shop, merch);
    unsigned int quantity_carts = quantity_in_carts(shop, merch);

    if ((int)(quantity_stock - quantity_carts) <= 0)
    {
        return false;
    }

    unsigned int available = quantity_stock - quantity_carts;
    if (amount > available)
    {
        return false;
    }

    cart_t *cart = cart_option.value.p;

    cart_item_t tmp = {.name = merch_name};
    ioopm_list_option_t cart_item_option = ioopm_linked_list_lookup(cart->items, ptr_elem(&tmp));
    if (Successful(cart_item_option))
    {
        cart_item_t *cart_item = cart_item_option.value.p;
        cart_item->quantity += amount;
    }
    else
    {
        ioopm_linked_list_append(cart->items, ptr_elem(create_cart_item(merch_name, amount)));
    }
    return true;
}

bool remove_item_from_cart(cart_t *cart, cart_item_t *item)
{
    assert(!pointer_is_null(cart));
    assert(!pointer_is_null(item));

    ioopm_list_iterator_t *iter = ioopm_list_iterator(cart->items);

    bool success = true;
    while (success)
    {
        cart_item_t *cart_item = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            if (strcmp(item->name, cart_item->name) == 0)
            {
                ioopm_iterator_remove(iter, &success);
                if (success == false)
                {
                    return false;
                }
                break;
            }
        }
        ioopm_iterator_next(iter, &success);
    }

    destroy_cart_item(item);
    ioopm_iterator_destroy(iter);
    return true;
}

bool ioopm_shop_remove_from_cart(ioopm_shop_t *shop, unsigned int cart_index, char *merch_name, unsigned int amount)
{
    if (amount == 0)
    {
        return false;
    }

    ioopm_option_t option = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem(merch_name));
    if (Unsuccessful(option))
    {
        return false; // om merchen ej existerar
    }

    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    if (Unsuccessful(cart_option))
    {
        return false; // om cart ej existerar
    }

    cart_t *cart = cart_option.value.p;

    cart_item_t tmp = {.name = merch_name};

    ioopm_list_option_t cart_item_option = ioopm_linked_list_lookup(cart->items, ptr_elem(&tmp));
    if (Unsuccessful(cart_item_option))
    {
        return false;
    }

    cart_item_t *item = cart_item_option.value.p;

    if ((item->quantity - amount) <= 0)
    {
        remove_item_from_cart(cart, item);
        return true;
    }

    item->quantity -= amount;

    return true;
}

// if the cart is empty or the cart doesn't exist it returns 0
unsigned int ioopm_shop_calculate_cost(ioopm_shop_t *shop, unsigned int cart_index)
{
    unsigned int cost = 0;
    ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(cart_index));
    if (Unsuccessful(cart_option))
    {
        return cost; // if the cart doesn't exist or is empty
    }

    cart_t *cart = cart_option.value.p;
    ioopm_list_t *items = cart->items;
    int cart_size = ioopm_linked_list_size(items);

    if (cart_size < 1)
    {
        return cost; // if the cart is empty
    }

    bool success;
    for (int i = 0; i < cart_size; i++)
    {
        cart_item_t *cart_item = ioopm_linked_list_get(items, i, &success).p;
        // om den hittar cart_item
        if (success)
        {
            char *item_name = cart_item->name;
            unsigned int quantity = cart_item->quantity;

            // letar efter merch i merch ht
            ioopm_option_t merch_option = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem(item_name));

            if (Successful(merch_option))
            {
                ioopm_merch_t *merch = merch_option.value.p;
                unsigned int price = merch->price;
                cost += quantity * price;
            }
        }
    }
    return cost;
}

void destroy_shelf_in_locs_ht(ioopm_shop_t *shop, ioopm_shelf_t *shelf)
{
    ioopm_option_t shelf_option = ioopm_hash_table_remove(shop->locs_ht, ptr_elem(shelf->name));
    assert(Successful(shelf_option));
    free(shelf_option.key.p);
    free(shelf_option.value.p);
}

/// @brief
/// @param merch
/// @return true if merch successfully decreased, otherwise false
void decrease_merch(ioopm_shop_t *shop, ioopm_merch_t *merch, unsigned int amount)
{
    ioopm_list_t *locs = merch->locs;

    bool success = false;
    for (size_t i = 0; i < ioopm_linked_list_size(locs); i++)
    {
        ioopm_shelf_t *shelf = ioopm_linked_list_get(locs, i, &success).p;
        if (success)
        {
            unsigned int shelf_quantity = shelf->quantity;

            int quantity_left_in_shelf = (int)(shelf_quantity - amount);

            if (quantity_left_in_shelf <= 0)
            {
                destroy_shelf_in_locs_ht(shop, shelf);
                free(shelf->name);
                free(shelf);
                ioopm_linked_list_remove(locs, i, &success);
                i--;
                amount -= shelf_quantity;
                continue;
            }

            shelf->quantity -= amount;
            break;
        }
    }
}

bool ioopm_shop_checkout_cart(ioopm_shop_t *shop, unsigned int cart_index)
{
    ioopm_option_t cart_option = ioopm_hash_table_remove(shop->shopping_carts->carts, u_elem(cart_index));
    if (Unsuccessful(cart_option))
    {
        return false;
    }

    cart_t *cart = cart_option.value.p;
    ioopm_list_t *items = cart->items;
    int cart_size = ioopm_linked_list_size(items);

    bool success;
    for (int i = 0; i < cart_size; i++)
    {
        cart_item_t *cart_item = ioopm_linked_list_get(items, i, &success).p;
        // om den hittar cart_item
        if (success)
        {
            char *item_name = cart_item->name;
            unsigned int quantity = cart_item->quantity;

            // letar efter merch i merch ht
            ioopm_option_t merch_option = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem(item_name));

            if (Successful(merch_option))
            {
                ioopm_merch_t *merch = merch_option.value.p;
                decrease_merch(shop, merch, quantity);
            }
        }
    }
    destroy_cart(cart);
    return true;
}
