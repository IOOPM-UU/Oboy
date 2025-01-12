#include "lib_common.h"
#include <assert.h>
#include "lib_hash_table.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lib_linked_list.h"

#define Success(k, v) \
    (lib_option_t) { .success = true, .key = k, .value = v }
#define Failure() \
    (lib_option_t) { .success = false }
#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)
#define No_Buckets 17

typedef struct lib_entry lib_entry_t;                 // entry_t is an alias for struct entry
typedef struct lib_hash_table lib_hash_table_t; // lib_hash_table_t är ett alias för struct lib_hash_table
typedef struct lib_option lib_option_t;

struct lib_entry
{
    lib_elem_t key;    // Key
    lib_elem_t value;  // Value
    lib_entry_t *next; // Pointer to the next entry, possibly NULL
};

struct lib_hash_table
{
    lib_entry_t buckets[No_Buckets];
    size_t size;
    lib_hash_function *hash_function;
    lib_eq_function *key_eq_func;
    lib_eq_function *value_eq_func;
};

static int default_hash_function(lib_elem_t value)
{
    return value.i;
}

lib_hash_table_t *lib_hash_table_create(lib_eq_function *key_eq_func, lib_eq_function *value_eq_func, lib_hash_function *hash_function)
{
    // Allocate space for a lib_hast_table_t = 17 pointers to entry_ts, which are set to NULL due to CALLOC
    lib_hash_table_t *result = calloc(1, sizeof(lib_hash_table_t));
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

static void entry_destroy(lib_entry_t *e)
{
    free(e);
}

void lib_hash_table_destroy(lib_hash_table_t *ht)
{
    lib_hash_table_clear(ht);
    free(ht);
}

static lib_entry_t *find_previous_entry_for_key(lib_hash_table_t *ht, lib_entry_t *e, lib_elem_t key)
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

// static lib_entry_t *find_previous_entry_for_key(lib_hash_table_t *ht, lib_entry_t *dummy, lib_elem_t key)
// {
//     lib_entry_t *cursor = dummy;

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

static lib_entry_t *entry_create(lib_elem_t key, lib_elem_t value, lib_entry_t *next)
{
    lib_entry_t *result = calloc(1, sizeof(lib_entry_t));
    result->key = key;
    result->value = value;
    result->next = next;
    return result;
}

void lib_hash_table_insert(lib_hash_table_t *ht, lib_elem_t key, lib_elem_t value) // first but buggy copied from instructions
{
    int hashed_key = ht->hash_function(key);
    int bucket = abs(hashed_key % No_Buckets);                                            // Calculate the bucket for this entry
    lib_entry_t *previous_entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key); // Search for an existing entry for a key
    lib_entry_t *next = previous_entry->next;

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

lib_option_t lib_hash_table_lookup(lib_hash_table_t *ht, lib_elem_t key)
{
    int hashed_key = ht->hash_function(key);
    int bucket = abs(hashed_key % No_Buckets);
    lib_entry_t *previous_entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);

    lib_entry_t *next_entry = previous_entry->next;
    if (next_entry != NULL)
    {
        return Success(next_entry->key, next_entry->value);
    }
    else
    {
        return Failure();
    }
}

lib_option_t lib_hash_table_remove(lib_hash_table_t *ht, lib_elem_t key)
{
    int hashed_key = ht->hash_function(key);
    int bucket = abs(hashed_key % No_Buckets);
    lib_entry_t *previous_entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);

    lib_entry_t *to_be_removed = previous_entry->next;

    if (to_be_removed != NULL)
    {
        previous_entry->next = to_be_removed->next;

        lib_entry_t tmp = *to_be_removed;

        entry_destroy(to_be_removed);

        ht->size -= 1;

        return Success(tmp.key, tmp.value);
    }
    else
    {
        return Failure();
    }
}

size_t lib_hash_table_size(lib_hash_table_t *ht)
{
    return ht->size;
}

bool lib_hash_table_is_empty(lib_hash_table_t *ht)
{
    return lib_hash_table_size(ht) == 0;
}

void lib_hash_table_clear(lib_hash_table_t *ht)
{
    if (lib_hash_table_is_empty(ht))
    {
        return;
    }

    for (int i = 0; i < No_Buckets; i++) // Loopar över alla buckets
    {
        lib_entry_t *first_entry = ht->buckets[i].next; // Första entryn i bucketen är en dummy entry, så den faktiskt första ges av detta uttryck
        lib_entry_t *current_entry = first_entry;
        while (current_entry != NULL) // Vi måste ta oss till den sista entryn innan vi börjar ta bort, för att undvika danling pointers och memory leaks
        {
            lib_entry_t *next_entry = current_entry->next;
            lib_hash_table_remove(ht, current_entry->key);
            current_entry = next_entry;
        }
    }
    ht->size = 0;
}

lib_list_t *lib_hash_table_keys(lib_hash_table_t *ht)
{
    lib_list_t *list = lib_linked_list_create(ht->key_eq_func); // denna callocar automatiskt

    for (int i = 0; i < No_Buckets; i++)
    {
        lib_entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        lib_entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            lib_linked_list_append(list, entry->key);
            lib_entry_t *next_entry = entry->next;
            entry = next_entry;
        }
    }
    return list;
}

lib_list_t *lib_hash_table_values(lib_hash_table_t *ht)
{
    lib_list_t *list = lib_linked_list_create(ht->value_eq_func); // denna callocar automatiskt

    for (int i = 0; i < No_Buckets; i++)
    {
        lib_entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        lib_entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            lib_linked_list_append(list, entry->value);
            lib_entry_t *next_entry = entry->next;
            entry = next_entry;
        }
    }
    return list;
}

bool lib_hash_table_any(lib_hash_table_t *ht, lib_predicate *pred, void *arg)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        lib_entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        lib_entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

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

bool lib_hash_table_all(lib_hash_table_t *ht, lib_predicate *pred, void *arg)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        lib_entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        lib_entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

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

bool lib_hash_table_has_key(lib_hash_table_t *ht, lib_elem_t key)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        lib_entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        lib_entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

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

bool lib_hash_table_has_value(lib_hash_table_t *ht, lib_elem_t value)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        lib_entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        lib_entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

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

void lib_hash_table_apply_to_all(lib_hash_table_t *ht, lib_apply_function *apply_fun, void *arg)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        lib_entry_t *dummy_entry = &ht->buckets[i]; // Första entryn i bucketen är en dummy entry
        lib_entry_t *entry = dummy_entry->next;     // Så den faktiskt första ges av detta uttryck

        while (entry != NULL)
        {
            apply_fun(entry->key, &entry->value, arg);
            entry = entry->next;
        }
    }
}