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

bool pointer_is_null(void *ptr)
{
    if (ptr == NULL)
    {
        printf("Error: pointer null");
        return true;
    }
    return false;
}

int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

bool string_eq(elem_t e1, elem_t e2)
{
    return (strcmp((char *)e1.p, (char *)e2.p) == 0);
}

bool merch_eq(elem_t e1, elem_t e2)
{
    ioopm_merch_t *merch1 = e1.p;
    ioopm_merch_t *merch2 = e2.p;

    return strcmp(merch1->name, merch2->name) == 0;
}

bool shelf_eq(elem_t e1, elem_t e2)
{
    ioopm_shelf_t *shelf1 = e1.p;
    ioopm_shelf_t *shelf2 = e2.p;

    return strcmp(shelf1->name, shelf2->name) == 0;
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

void destroy_cart_item(cart_item_t *cart_item)
{
    free(cart_item->name);
    free(cart_item);
}
