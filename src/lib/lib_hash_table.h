#pragma once
#include "lib_common.h"
#include <stdbool.h>
#include <stddef.h>
#include "lib_linked_list.h"

/**
 * @file hash_table.h
 * @author Max Ohlson, Emilio Funes
 * @date 16 Sep 2022
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */

typedef struct lib_hash_table lib_hash_table_t;
typedef struct lib_option lib_option_t;
typedef int lib_hash_function(lib_elem_t value); // TODO: int?


struct lib_option
{
    bool success;
    lib_elem_t key;
    lib_elem_t value;
};


/// @brief Create a new hash table
/// @return A new empty hash table
lib_hash_table_t *lib_hash_table_create(lib_eq_function *key_eq_func, lib_eq_function *value_eq_func, lib_hash_function* hash_function);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void lib_hash_table_destroy(lib_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void lib_hash_table_insert(lib_hash_table_t *ht, lib_elem_t key, lib_elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the key, value and a bool together as an option
lib_option_t lib_hash_table_lookup(lib_hash_table_t *ht, lib_elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the key, value and a bool together as an option
lib_option_t lib_hash_table_remove(lib_hash_table_t *ht, lib_elem_t key);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t lib_hash_table_size(lib_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true if size == 0, else false
bool lib_hash_table_is_empty(lib_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void lib_hash_table_clear(lib_hash_table_t *ht);

/// @note if key contains pointer to allocated memory, then returned entries in list will point to same adress (not allocate and duplicate)
/// @brief return the keys for all entries in a hash map (in no particular order, but same as lib_hash_table_values)
/// @param h hash table operated upon
/// @return an linked list of keys for hash table h
lib_list_t *lib_hash_table_keys(lib_hash_table_t *ht);

/// @note if key contains pointer to allocated memory, then returned entries in list will point to same adress (not allocate and duplicate)
/// @brief return the values for all entries in a hash map (in no particular order, but same as lib_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
lib_list_t *lib_hash_table_values(lib_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool lib_hash_table_has_key(lib_hash_table_t *ht, lib_elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool lib_hash_table_has_value(lib_hash_table_t *ht, lib_elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if all entries satisfy the predicate, otherwise false
bool lib_hash_table_all(lib_hash_table_t *ht, lib_predicate *pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool lib_hash_table_any(lib_hash_table_t *ht, lib_predicate *pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void lib_hash_table_apply_to_all(lib_hash_table_t *ht, lib_apply_function *apply_fun, void *arg);