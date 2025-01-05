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

void destroy_merch_locations(ioopm_hash_table_t *locs_ht, ioopm_merch_t *merch)
{
    ioopm_list_iterator_t *iter = ioopm_list_iterator(merch->locs);

    bool success = true;
    while (success)
    {
        ioopm_shelf_t *shelf = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            ioopm_option_t option = ioopm_hash_table_remove(locs_ht, ptr_elem(shelf->name));
            if (Successful(option))
            {
                free(option.key.p);
                free(option.value.p);
            }
            free(shelf->name);
            free(shelf);
        }
        ioopm_iterator_next(iter, &success);
    }
    ioopm_iterator_destroy(iter);
}

void remove_merch_from_cart(cart_t *cart, ioopm_merch_t *merch)
{
    ioopm_list_iterator_t *iter = ioopm_list_iterator(cart->items);

    bool success = true;
    while (success)
    {
        cart_item_t *cart_item = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            if (strcmp(merch->name, cart_item->name) == 0)
            {
                destroy_cart_item(cart_item);
                ioopm_iterator_remove(iter, &success);
            }
        }
        ioopm_iterator_next(iter, &success);
    }
    ioopm_iterator_destroy(iter);
}

void remove_merch_from_all_carts(shopping_carts_t *shopping_carts, ioopm_merch_t *merch)
{
    ioopm_list_t *carts = ioopm_hash_table_values(shopping_carts->carts);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(carts);

    bool success = true;
    while (success)
    {
        cart_t *cart = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            remove_merch_from_cart(cart, merch);
        }
        ioopm_iterator_next(iter, &success);
    }

    ioopm_linked_list_destroy(carts);
    ioopm_iterator_destroy(iter);
}

void destroy_merch(ioopm_merch_t *merch)
{
    free(merch->name);
    free(merch->description);
    ioopm_linked_list_destroy(merch->locs);
    free(merch);
}

bool ioopm_shop_remove_merch(ioopm_shop_t *shop, char *merch_name)
{
    if (pointer_is_null(shop))
    {
        return false;
    }

    ioopm_option_t option = ioopm_hash_table_remove(shop->merch_ht, ptr_elem(merch_name));
    if (Unsuccessful(option))
    {
        return false;
    }

    free(option.key.p);

    ioopm_merch_t *merch = option.value.p;

    remove_merch_from_all_carts(shop->shopping_carts, merch);

    destroy_merch_locations(shop->locs_ht, merch);

    destroy_merch(merch);

    return true;
}

bool ioopm_shop_add_merch(ioopm_shop_t *shop, char *name, char *desc, unsigned int price)
{
    if (pointer_is_null(shop))
    {
        return false;
    }

    ioopm_option_t result = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem(name));

    if (Successful(result))
    {
        return false;
    }

    ioopm_merch_t *new_merch = calloc(1, sizeof(ioopm_merch_t));

    char *name_alloc = strdup(name);
    char *desc_alloc = strdup(desc);

    new_merch->name = name_alloc;
    new_merch->description = desc_alloc;
    new_merch->price = price;
    new_merch->locs = ioopm_linked_list_create(shelf_eq);

    // Allocate memory for ht key string
    char *key = strdup(name);

    ioopm_hash_table_insert(shop->merch_ht, ptr_elem(key), ptr_elem(new_merch));

    return true;
}

ioopm_list_t *ioopm_shop_get_merch(ioopm_shop_t *shop)
{
    ioopm_list_t *all_merch = ioopm_hash_table_keys(shop->merch_ht);
    size_t merch_count = ioopm_linked_list_size(all_merch);

    // // skapar en array av str för qsort
    char **merch_names = calloc(merch_count, sizeof(char *));

    ioopm_list_iterator_t *iter = ioopm_list_iterator(all_merch);

    bool success = false;
    for (int i = 0; i < merch_count; i++)
    {
        elem_t elem = ioopm_iterator_current(iter, &success);

        assert(success); // Crash if current fails

        ioopm_iterator_next(iter, &success);

        merch_names[i] = (char *)(elem.p);
    }
    sort_keys(merch_names, merch_count);

    ioopm_list_t *sorted_merch = ioopm_linked_list_create(string_eq);

    for (int i = 0; i < merch_count; i++)
    {
        ioopm_linked_list_append(sorted_merch, ptr_elem(merch_names[i]));
    }

    ioopm_linked_list_destroy(all_merch);
    free(merch_names);

    ioopm_iterator_destroy(iter);

    return sorted_merch;
}

static void change_name_cart_item(ioopm_shop_t *shop, char *current_name, char *new_name)
{
    unsigned int cart_index = shop->shopping_carts->cart_index;
    assert(shop->shopping_carts->carts);
    for (int i = 0; i < cart_index; i++) // kollar genom hela hashtable
    {
        ioopm_option_t cart_option = ioopm_hash_table_lookup(shop->shopping_carts->carts, u_elem(i));
        if (Successful(cart_option))
        {
            cart_t *cart = cart_option.value.p;
            ioopm_list_t *items = cart->items;
            cart_item_t tmp = {.name = current_name};
            ioopm_list_option_t items_option = ioopm_linked_list_lookup(items, ptr_elem(&tmp));
            if (Successful(items_option))
            {
                cart_item_t *cart_item = items_option.value.p;
                free(cart_item->name);
                cart_item->name = strdup(new_name);
            }
        }
    }
}

bool ioopm_shop_edit_merchandise(ioopm_shop_t *shop, char *old_name, char *new_name, char *new_description, unsigned int new_price)
{
    if (pointer_is_null(shop))
    {
        return false;
    }

    // checking if the new name already exists in the shop
    ioopm_option_t existing = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem(new_name));
    if (Successful(existing))
    {
        return false;
    }

    ioopm_option_t option = ioopm_hash_table_remove(shop->merch_ht, ptr_elem(old_name));
    if (Unsuccessful(option))
    {
        return false;
    }

    char *ht_key = (char *)option.key.p;
    free(ht_key);

    ioopm_merch_t *merch = option.value.p;

    free(merch->name);
    free(merch->description);

    char *new_name_alloc = strdup(new_name);
    char *new_desc_alloc = strdup(new_description);

    merch->name = new_name_alloc;
    merch->description = new_desc_alloc;
    merch->price = new_price;

    char *new_key = strdup(new_name);

    ioopm_hash_table_insert(shop->merch_ht, ptr_elem(new_key), ptr_elem(merch));

    ioopm_list_iterator_t *iter = ioopm_list_iterator(merch->locs);

    bool success = false;
    while (ioopm_iterator_has_next(iter) || success)
    {
        ioopm_shelf_t *shelf = ioopm_iterator_current(iter, &success).p;
        char *shelf_name = shelf->name;
        ioopm_option_t option = ioopm_hash_table_lookup(shop->locs_ht, ptr_elem(shelf_name));

        if (Successful(option))
        {
            free(option.value.p);

            char *name_alloc_locs = strdup(new_name);
            option.value.p = name_alloc_locs;
        }
        ioopm_iterator_next(iter, &success);
    }

    change_name_cart_item(shop, old_name, new_name);

    ioopm_iterator_destroy(iter);
    return true;
}

static ioopm_shelf_t *create_shelf(char *shelf_name, unsigned int shelf_quantity)
{
    ioopm_shelf_t *shelf = calloc(1, sizeof(ioopm_shelf_t));
    assert(shelf);
    shelf->name = strdup(shelf_name);
    shelf->quantity = shelf_quantity;
    return shelf;
}

bool ioopm_shop_insert_stock(ioopm_shop_t *shop, char *merch_name, char *shelf_name, unsigned int shelf_quantity)
{
    if (shelf_quantity < 1)
    {
        return false;
    }

    ioopm_option_t loc_option = ioopm_hash_table_lookup(shop->locs_ht, ptr_elem(shelf_name));
    if (Successful(loc_option))
    {
        if (strcmp((char *)loc_option.value.p, merch_name) != 0)
        {
            return false;
        }
    }

    ioopm_option_t option = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem(merch_name));
    if (Unsuccessful(option))
    {
        return false;
    }

    ioopm_list_t *locs = ((ioopm_merch_t *)option.value.p)->locs;

    ioopm_shelf_t tmp = (ioopm_shelf_t){.name = shelf_name};
    ioopm_list_option_t option_list = ioopm_linked_list_lookup(locs, ptr_elem(&tmp));

    if (Successful(option_list))
    {
        ioopm_shelf_t *shelf = option_list.value.p;
        shelf->quantity += shelf_quantity;
    }
    else
    {
        ioopm_shelf_t *shelf = create_shelf(shelf_name, shelf_quantity);
        ioopm_linked_list_append(locs, ptr_elem(shelf));
        ioopm_hash_table_insert(shop->locs_ht, ptr_elem(strdup(shelf_name)), ptr_elem(strdup(merch_name)));
    }
    return true;
}

ioopm_list_t *ioopm_shop_get_stock(ioopm_shop_t *shop, char *merch_name)
{
    ioopm_option_t option = ioopm_hash_table_lookup(shop->merch_ht, ptr_elem(merch_name));
    if (Unsuccessful(option))
    {
        return NULL;
    }
    ioopm_list_t *locs = ((ioopm_merch_t *)option.value.p)->locs;
    return ioopm_linked_list_duplicate(locs);
}

char *ioopm_shop_get_shelf_name(ioopm_shelf_t *shelf)
{
    return shelf->name;
}

unsigned int ioopm_shop_get_shelf_quantity(ioopm_shelf_t *shelf)
{
    return shelf->quantity;
}

void destroy_shelfs_in_list(ioopm_list_t *list)
{
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

    bool success = true;
    while (success)
    {
        ioopm_shelf_t *shelf = ioopm_iterator_current(iter, &success).p;
        if (success)
        {
            free(shelf->name);
            free(shelf);
        }
        ioopm_iterator_next(iter, &success);
    }
    ioopm_iterator_destroy(iter);
}

void destroy_all_merch(ioopm_shop_t *shop)
{
    ioopm_list_t *merch_keys = ioopm_hash_table_keys(shop->merch_ht);
    ioopm_list_t *merch_values = ioopm_hash_table_values(shop->merch_ht);
    ioopm_list_t *locs_keys = ioopm_hash_table_keys(shop->locs_ht);
    ioopm_list_t *locs_values = ioopm_hash_table_values(shop->locs_ht);

    ioopm_list_iterator_t *iter_mk = ioopm_list_iterator(merch_keys);
    ioopm_list_iterator_t *iter_mv = ioopm_list_iterator(merch_values);
    ioopm_list_iterator_t *iter_lk = ioopm_list_iterator(locs_keys);
    ioopm_list_iterator_t *iter_lv = ioopm_list_iterator(locs_values);

    ioopm_hash_table_destroy(shop->merch_ht);
    ioopm_hash_table_destroy(shop->locs_ht);

    bool success_k = true, success_v = true;
    while (success_k && success_v)
    {
        void *k_ptr = ioopm_iterator_current(iter_mk, &success_k).p;
        ioopm_merch_t *v_ptr = ioopm_iterator_current(iter_mv, &success_v).p;
        if (success_k)
        {
            free(k_ptr);
        }
        if (success_v)
        {
            free(v_ptr->name);
            free(v_ptr->description);
            destroy_shelfs_in_list(v_ptr->locs);
            ioopm_linked_list_destroy(v_ptr->locs);
            free(v_ptr);
        }
        ioopm_iterator_next(iter_mk, &success_k);
        ioopm_iterator_next(iter_mv, &success_v);
    }

    success_k = true, success_v = true;
    while (success_k && success_v)
    {
        void *k_ptr = ioopm_iterator_current(iter_lk, &success_k).p;
        void *v_ptr = ioopm_iterator_current(iter_lv, &success_v).p;
        if (success_k)
        {
            free(k_ptr);
        }
        if (success_v)
        {
            free(v_ptr);
        }
        ioopm_iterator_next(iter_lk, &success_k);
        ioopm_iterator_next(iter_lv, &success_v);
    }

    ioopm_linked_list_destroy(merch_keys);
    ioopm_linked_list_destroy(merch_values);
    ioopm_linked_list_destroy(locs_keys);
    ioopm_linked_list_destroy(locs_values);
    ioopm_iterator_destroy(iter_mk);
    ioopm_iterator_destroy(iter_mv);
    ioopm_iterator_destroy(iter_lk);
    ioopm_iterator_destroy(iter_lv);
}