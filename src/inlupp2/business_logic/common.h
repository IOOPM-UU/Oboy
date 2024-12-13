#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "backend.h"
#include "../generic_data_structures/common.h"

#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)

typedef struct shelf ioopm_shelf_t;
typedef struct merch ioopm_merch_t;
typedef struct shopping_carts shopping_carts_t;
typedef struct cart cart_t;
typedef struct cart_item cart_item_t;


/// @brief Represents the entire shop and its implementations. It holds two 
///hash tables: 
///-merch_ht -A hash table mapping merchandise names to their information
///-locs_ht  -A hash table mapping shelf names to the merchandise name that
/// it holds.
/// It also contains a shopping_carts struct, which tracks all active customer
/// shopping carts.
struct shop
{
    ioopm_hash_table_t *merch_ht;
    ioopm_hash_table_t *locs_ht;
    shopping_carts_t *shopping_carts;
};

/// @brief Represents a specific merchandise item in the shop.
///
/// This struct contains the basic properties of a product:
/// - `name`: The name of the merchandise.
/// - `description`: A brief description of the merchandise.
/// - `price`: The price of the merchandise.
/// - `locs`: A list of shelves (`ioopm_list_t`) where this merchandise is stored.
struct merch
{
    char *name;
    char *description;
    unsigned int price;
    ioopm_list_t *locs;
};


/// @brief Represents a shelf in the shop.
///
/// This struct contains:
/// - `name`: The name of the shelf (e.g., A35, B12).
/// - `quantity`: The amount of merchandise stored on this shelf.
struct shelf
{
    char *name;
    unsigned int quantity;
};

/// @brief Represents all active shopping carts in the shop.
///
/// This struct contains:
/// - `cart_index`: A unique identifier for the most recently created cart.
/// - `carts`: A hash table that maps cart indices to `cart_t` structs, 
///representing individual carts.
struct shopping_carts
{
    unsigned int cart_index;
    ioopm_hash_table_t *carts;
};


/// @brief Represents an individual shopping cart.
///
/// This struct contains a list of items (`ioopm_list_t`), where each item is 
///represented by a `cart_item_t` struct.
struct cart
{
    ioopm_list_t *items;
};

/// @brief Represents an item in a shopping cart.
///
/// This struct contains:
/// - `name`: The name of the merchandise.
/// - `quantity`: The number of units of this merchandise added to the cart.
struct cart_item
{
    char *name;
    unsigned int quantity;
};

/// @brief Checks if the pointer is null
/// @param ptr a void pointer
/// @return bool, true if the pointer is null, false otherwise
bool pointer_is_null(void *ptr);

/// @brief Creates a shopping_cart struct, allocating necessary memory
/// @return shopping_carts_t, a shopping cart struct
shopping_carts_t *create_shopping_cart(void);

/// @brief Destroys a shopping cart, deallocating its memory
/// @param shopping_carts, the shopping cart to be destroyed
void destroy_shopping_cart(shopping_carts_t *shopping_carts);

/// @brief Destroys all merch from a shop
/// @param shop  the shop struct
void destroy_all_merch(ioopm_shop_t *shop);

/// @brief Compares two strings, returns 0 if they are equal
///other value if not
/// @param p1 first string
/// @param p2 second string
/// @return an int, 0 if they are equal, other value otherwise
int cmpstringp(const void *p1, const void *p2);

/// @brief Sorts an array of strings in alphabetical order (in place)
/// @param keys array of string
/// @param no_keys number of keys
void sort_keys(char *keys[], size_t no_keys);

/// @brief Compare function for strings
/// @param e1 elem_t cointaining a pointer to a char
/// @param e2 elem_t cointaining a pointer to a char
/// @return bool, true if the strings are equal, false if not
bool string_eq(elem_t e1, elem_t e2);

/// @brief Compare function for merch, it compares the merch name
/// @param e1 elem_t containing a pointer to a merch
/// @param e2 elem_t containing a pointer to a merch
/// @return bool, true if the merch names are equal, false if not
bool merch_eq(elem_t e1, elem_t e2);

/// @brief Compare function for shelf, it compares the shelf name
/// @param e1 elem_t containing a pointer to a shelf
/// @param e2 elem_t containing a pointer to a shelf
/// @return bool, true if the merch names are equal, false if not
bool shelf_eq(elem_t e1, elem_t e2);

/// @brief Hash function for strings, returns a specific integer
///to be mapped in a specific bucket in the hash table
/// @param e elem_t containing a pointer to a string
/// @return int, integer
int string_sum_hash(elem_t e);

/// @brief Frees all the memory attached to a cart item
/// @param cart_item the cart_item to be destroyed
void destroy_cart_item(cart_item_t *cart_item);
