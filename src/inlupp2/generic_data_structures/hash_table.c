#include "common.h"
#include <assert.h>
#include "hash_table.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"

#define Success(k, v) \
    (ioopm_option_t) { .success = true, .key = k, .value = v }
#define Failure() \
    (ioopm_option_t) { .success = false }
#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)
#define No_Buckets 17

typedef struct entry entry_t;                 // entry_t is an alias for struct entry
typedef struct hash_table ioopm_hash_table_t; // ioopm_hash_table_t är ett alias för struct ioopm_hash_table
typedef struct option ioopm_option_t;

struct entry
{
    elem_t key;    // Key
    elem_t value;  // Value
    entry_t *next; // Pointer to the next entry, possibly NULL
};

struct hash_table
{
    entry_t buckets[No_Buckets];
    size_t size;
    ioopm_hash_function *hash_function;
    ioopm_eq_function *key_eq_func;
    ioopm_eq_function *value_eq_func;
};

static int default_hash_function(elem_t value)
{
    return value.i;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function *key_eq_func, ioopm_eq_function *value_eq_func, ioopm_hash_function *hash_function)
{
    // Allocate space for a ioopm_hast_table_t = 17 pointers to entry_ts, which are set to NULL due to CALLOC
    ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
    result->size = 0;
    result->key_eq_func = key_eq_func;
    result->value_eq_func = value_eq_func;
    if (hash_function != NULL)
    {
        result->hash_function = hash_function;
    }
    else
    {
        result->hash_function = default_hash_function;
    }
    return result;
}

static void entry_destroy(entry_t *e)
{
    free(e);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    ioopm_hash_table_clear(ht);
    free(ht);
}

static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *e, elem_t key)
{
    if (e->next == NULL || ht->key_eq_func(e->next->key, key))
    {
        return e;
    }
    else
    {
        return find_previous_entry_for_key(ht, e->next, key);
    }
}

// static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *dummy, elem_t key)
// {
//     entry_t *cursor = dummy;

//     while (cursor->next != NULL)
//     {
//         if (cursor->next == NULL || ht->key_eq_func(cursor->next->key, key))
//         {
//             return cursor;
//         }
//         cursor = cursor->next;
//     }
//     return cursor;
// }

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
    entry_t *result = calloc(1, sizeof(entry_t));
    result->key = key;
    result->value = value;
    result->next = next;
    return result;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) // first but buggy copied from instructions
{
    int hashed_key = ht->hash_function(key);
    int bucket = abs(hashed_key % No_Buckets);                                            // Calculate the bucket for this entry
    entry_t *previous_entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key); // Search for an existing entry for a key
    entry_t *next = previous_entry->next;

    /// Check if the next entry should be updated or not
    if (next != NULL)
    {
        if (ht->key_eq_func(next->key, key))
        {
            next->value = value;
            return;
        }
    }

    previous_entry->next = entry_create(key, value, next);
    ht->size += 1;
}

ioopm_option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
    int hashed_key = ht->hash_function(key);
    int bucket = abs(hashed_key % No_Buckets);
    entry_t *previous_entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);

    entry_t *next_entry = previous_entry->next;
    if (next_entry != NULL)
    {
        return Success(next_entry->key, next_entry->value);
    }
    else
    {
        return Failure();
    }
}

ioopm_option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
    int hashed_key = ht->hash_function(key);
    int bucket = abs(hashed_key % No_Buckets);
    entry_t *previous_entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);

    entry_t *to_be_removed = previous_entry->next;

    if (to_be_removed != NULL)
    {
        previous_entry->next = to_be_removed->next;

        entry_t tmp = *to_be_removed;

        entry_destroy(to_be_removed);

        ht->size -= 1;

        return Success(tmp.key, tmp.value);
    }
    else
    {
        return Failure();
    }
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
    return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
    return ioopm_hash_table_size(ht) == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
    if (ioopm_hash_table_is_empty(ht))
    {
        return;
    }

    for (int i = 0; i < No_Buckets; i++) // Loopar över alla buckets
    {
        entry_t *first_entry = ht->buckets[i].next; // Första entryn i bucketen är en dummy entry, så den faktiskt första ges av detta uttryck
        entry_t *current_entry = first_entry;
        while (current_entry != NULL) // Vi måste ta oss till den sista entryn innan vi börjar ta bort, för att undvika danling pointers och memory leaks
        {
            entry_t *next_entry = current_entry->next;
            ioopm_hash_table_remove(ht, current_entry->key);
            current_entry = next_entry;
        }
    }
    ht->size = 0;
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
    ioopm_list_t *list = ioopm_linked_list_create(ht->key_eq_func); // denna callocar automatiskt

    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            ioopm_linked_list_append(list, entry->key);
            entry_t *next_entry = entry->next;
            entry = next_entry;
        }
    }
    return list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
    ioopm_list_t *list = ioopm_linked_list_create(ht->value_eq_func); // denna callocar automatiskt

    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            ioopm_linked_list_append(list, entry->value);
            entry_t *next_entry = entry->next;
            entry = next_entry;
        }
    }
    return list;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            if (pred(entry->key, entry->value, arg))
            {
                return true;
            }

            entry = entry->next;
        }
    }
    return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            if (!(pred(entry->key, entry->value, arg)))
            {
                return false;
            }
            entry = entry->next;
        }
    }
    return true;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            if (ht->key_eq_func(entry->key, key))
            {
                return true;
            }

            entry = entry->next;
        }
    }
    return false;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            if (ht->value_eq_func(value, entry->value))
            {
                return true;
            }

            entry = entry->next;
        }
    }
    return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_fun, void *arg)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            apply_fun(entry->key, &entry->value, arg);
            entry = entry->next;
        }
    }
}