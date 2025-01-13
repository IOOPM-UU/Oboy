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
#include "../../src/refmem.h"

void shop_destructor(obj *shop) {
    if (!shop) return;
    release(((ioopm_shop_t *)shop)->merch_ht);
    release(((ioopm_shop_t *)shop)->locs_ht);
    release(((ioopm_shop_t *)shop)->shopping_carts);
}

ioopm_shop_t *ioopm_create_shop()
{
    ioopm_shop_t *shop = allocate(sizeof(ioopm_shop_t), shop_destructor);
    if (shop == NULL)
    {
        printf("Failed to allocate memory");
        assert(false);
    }
    shop->merch_ht = ioopm_hash_table_create(string_eq, merch_eq, string_sum_hash);
    retain(shop->merch_ht);
    shop->locs_ht = ioopm_hash_table_create(string_eq, shelf_eq, string_sum_hash);
    retain(shop->locs_ht);
    shop->shopping_carts = create_shopping_cart();
    retain(shop->shopping_carts);
    return shop;
}

void ioopm_shop_destroy(ioopm_shop_t *shop)
{
    destroy_shopping_cart(shop->shopping_carts);
    destroy_all_merch(shop);
    release(shop);
}

