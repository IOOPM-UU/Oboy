#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../generic_data_structures/common.h"
#include "../generic_data_structures/linked_list.h"
#include "../generic_data_structures/hash_table.h"
#include "../generic_data_structures/iterator.h"
#include "../generic_utils/utils.h"


/**
 * @file backend.h
 * @author Emilio Funes, Alexandra Barbu
 * @date 20 Oct 2024
 * @brief Interface for managing a shop's inventory, merchandise, and customer carts.
 *
 * This header file defines the core operations for managing a shop's inventory system. 
 * It provides the function declarations for creating, modifying, and removing merchandise, 
 * handling stock levels, managing customer shopping carts, and interacting with the shop's 
 * data. The shop structure consists of two hash tables: one that maps merchandise to its 
 * details (such as name, description, and price) and another that maps shelves to the 
 * merchandise they hold. The file also defines operations for cart management, 
 * including adding or removing items from carts and handling multiple customer carts.
 *
 */


/// @brief Struct shop, that contains all the data about the shop
typedef struct shop ioopm_shop_t;

typedef struct merch ioopm_merch_t;

typedef struct shelf ioopm_shelf_t;

///@brief Creates a stock, which is a struct including two hash tables, one
/// that maps merch to its details, and another one that maps each location
/// to the merch it contains
///@return ioopm_shop_t, a stock with two hash tables
ioopm_shop_t *ioopm_create_shop();

/// @brief The function adds merch to a stock, it is represented by a loop takes
/// the input needed (name, description, price). The new added item is not in stock.
/// Adding a new merch with the same name as an existing merch is not allowed.
/// @param shop the shop struct
/// @param name the merch name
/// @param desc the merch description
/// @param price the merch price
/// @return true if merch was added, false if not
bool ioopm_shop_add_merch(ioopm_shop_t *shop, char *name, char *desc, unsigned int price);

/// @brief Lists all the items in the store in alphabetical order
/// @param shop  the shop struct
ioopm_list_t *ioopm_shop_get_merch(ioopm_shop_t *shop);

/// @brief Removes merchandise with specified name
/// @param shop the shop struct
/// @param name name of merch to be removed
bool ioopm_shop_remove_merch(ioopm_shop_t *shop, char *merch_name);

/// @brief Delets a shop and frees its memory
/// @param shop a shop to be deleted
void ioopm_shop_destroy(ioopm_shop_t *shop);

/// @brief Changes the name, description and price of a merch, note that
///this doesn't affect the merch's stock
/// @param shop the shop struct
/// @param old_name the current name of the merch
/// @param new_name new desired name of the merch
/// @param new_description new description of the merch
/// @param new_price  new price of the merch
/// @return bool, if the changing was succesfull or not
bool ioopm_shop_edit_merchandise(ioopm_shop_t *shop, char *old_name, char *new_name, char *new_description, unsigned int new_price);

/// @brief Increases the stock of a merch by at least one, if the shelf given
///does not exist, then a new one is created and added into the shop
/// @param shop the shop struct
/// @param merch_name name of the merch which stock should be replenished
/// @param shelf_name  name of the shelf on which stock should be replenished
/// @param shelf_quantity amount of stock desired to be added
/// @return bool, if the inserting was succesfull or not
bool ioopm_shop_insert_stock(ioopm_shop_t *shop, char *merch_name, char *shelf_name, unsigned int shelf_quantity);

/// @brief Gets all the stock for a specific merch along with the quantities
///available on each shelf.
/// @param shop the shop struct
/// @param merch_name name of the merch which stock is wanted
/// @return NULL if merch not found otherwise a list with all stock and informations about it
ioopm_list_t *ioopm_shop_get_stock(ioopm_shop_t *shop, char *merch_name);

/// @brief Creates a new empty cart
/// @param shop the shop struct
/// @return int, an identifying number for the cart
unsigned int ioopm_shop_create_cart(ioopm_shop_t *shop);

/// @brief Removes a cart from the system
/// @param shop the shop struct
/// @param index the identifying number of the cart to be removed
/// @return bool, if the operation was succesful or not
bool ioopm_shop_remove_cart(ioopm_shop_t *shop, unsigned int index);

/// @brief Adds some quantity of a merch into a specific cart, where all 
///possible orders must be fulfillable (multiple carts exist).
/// @param shop the shop struct
/// @param cart_index the identifying number of the cart the items should
///be added to
/// @param merch_name name of the merch to add to cart
/// @param amount amount of merch to add to cart
/// @return bool, if the operation was succesful or not
bool ioopm_shop_add_to_cart(ioopm_shop_t *shop, unsigned int cart_index, char *merch_name, unsigned int amount);

/// @brief Removes zero or more items of some specific merch from the cart.
/// @param shop the shop struct
/// @param cart_index the identifying number of the cart
/// @param merch_name name of the merch which items wish to be removed
/// @param amount number of items wished to be removed
/// @return bool, if the operation was succesfull or not
bool ioopm_shop_remove_from_cart(ioopm_shop_t *shop, unsigned int cart_index, char *merch_name, unsigned int amount);

/// @brief Calculates the total cost of a cart, based on the identifying index,
///if the cart is empty or non-exisitng, it will return 0.
/// @param shop the shop struct
/// @param cart_index the identifying index of the cart
/// @return unsigned int, the total cost (0 if empty or NULL)
unsigned int ioopm_shop_calculate_cost(ioopm_shop_t *shop, unsigned int cart_index);

/// @brief The user goes through with a purchase, decreases the stock from the merch 
///in the cart and removes the shopping cart from the system
/// @param shop the shop struct
/// @param cart_index the identifying index of the cart
/// @return bool if the operation succedded or not
bool ioopm_shop_checkout_cart(ioopm_shop_t *shop, unsigned int cart_index);

/// @brief Helper function for frontend to get the name of a shelf
/// @param shelf the shelf struct
/// @return char, the name of the shelf
char *ioopm_shop_get_shelf_name(ioopm_shelf_t *shelf);


/// @brief Helper function for frontend to get the quantity of a shelf
/// @param shelf the shelf struct
/// @return unsigned int, the quantity of a merch on the shelf
unsigned int ioopm_shop_get_shelf_quantity(ioopm_shelf_t *shelf);