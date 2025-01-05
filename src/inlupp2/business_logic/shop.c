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

ioopm_shop_t *ioopm_create_shop()
{
    ioopm_shop_t *shop = calloc(1, sizeof(ioopm_shop_t));
    if (shop == NULL)
    {
        printf("Failed to allocate memory");
        assert(false);
    }
    shop->merch_ht = ioopm_hash_table_create(string_eq, merch_eq, string_sum_hash);
    shop->locs_ht = ioopm_hash_table_create(string_eq, shelf_eq, string_sum_hash);
    shop->shopping_carts = create_shopping_cart();
    return shop;
}

void ioopm_shop_destroy(ioopm_shop_t *shop)
{
    destroy_shopping_cart(shop->shopping_carts);
    destroy_all_merch(shop);
    free(shop);
}

