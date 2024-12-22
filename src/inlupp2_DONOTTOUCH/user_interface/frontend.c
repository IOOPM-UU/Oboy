#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../business_logic/backend.h"
#include "../generic_data_structures/common.h"
#include "../generic_data_structures/linked_list.h"
#include "../generic_data_structures/hash_table.h"
#include "../generic_data_structures/iterator.h"
#include "../generic_utils/utils.h"

#define ValidOptions "AaLlDdEeSsPpCcRr+-=OoQq"

void add_merchendice(ioopm_shop_t *shop);
void list_merchendice(ioopm_shop_t *shop);
void remove_merchandise(ioopm_shop_t *shop);
void edit_merchandise(ioopm_shop_t *shop);
void show_stock(ioopm_shop_t *shop);
void replenish_stock(ioopm_shop_t *shop);
void create_cart(ioopm_shop_t *shop);
void remove_cart(ioopm_shop_t *shop);
void add_to_cart(ioopm_shop_t *shop);
void remove_from_cart(ioopm_shop_t *shop);
void calculate_cost(ioopm_shop_t *shop);
void checkout(ioopm_shop_t *shop);

void quit_shop(bool *is_running);

// true if user presses y, otherwise false
// ignores case
bool user_confirmation();

/// @note s2 needs to be lower case
/// @brief compares first char in s1 with s2 and ignores case
/// @param s1 char to compare with s2
/// @param s2 char to compare s1 with
/// @return returns true if first char in s1 equals s2 (ignores case), otherwise false
bool char_eq_ignore_case(char *s1, char *s2)
{
    s1[0] = (char)tolower(s1[0]);
    return strcmp(s1, s2) == 0;
}

bool everything_allowed(char *input)
{
    return true;
}

bool vaild_option(char *input)
{
    char *valid_options = ValidOptions;

    if (strlen(input) != 1)
    {
        return false;
    }

    for (int i = 0; i < strlen(valid_options); i++)
    {
        if (input[0] == valid_options[i])
        {
            return true;
        }
    }

    return false;
}

char *ask_question_menu_option(char *question)
{
    return ask_question(question, vaild_option, (convert_func *)strdup).string_value;
}

void process_input(char *input, ioopm_shop_t *shop, bool *is_running)
{
    if (char_eq_ignore_case(input, "a"))
    {
        add_merchendice(shop);
    }
    else if (char_eq_ignore_case(input, "l"))
    {
        list_merchendice(shop);
    }
    else if (char_eq_ignore_case(input, "d"))
    {
        remove_merchandise(shop);
    }
    else if (char_eq_ignore_case(input, "e"))
    {
        edit_merchandise(shop);
    }
    else if (char_eq_ignore_case(input, "s"))
    {
        show_stock(shop);
    }
    else if (char_eq_ignore_case(input, "p"))
    {
        replenish_stock(shop);
    }
    else if (char_eq_ignore_case(input, "c"))
    {
        create_cart(shop);
    }
    else if (char_eq_ignore_case(input, "r"))
    {
        remove_cart(shop);
    }
    else if (char_eq_ignore_case(input, "+"))
    {
        add_to_cart(shop);
    }
    else if (char_eq_ignore_case(input, "-"))
    {
        remove_from_cart(shop);
    }
    else if (char_eq_ignore_case(input, "="))
    {
        calculate_cost(shop);
    }
    else if (char_eq_ignore_case(input, "o"))
    {
        checkout(shop);
    }
    else if (char_eq_ignore_case(input, "q"))
    {
        quit_shop(is_running);
    }
}

int main(void)
{
    char *menu_print = "A. Add merchandise \nL. List merchandise\nD. Remove merchandise\nE. Edit merchandise\nS. Show stock\nP. Replenish\nC. Create cart\nR. Remove cart\n+. Add to cart\n-. Remove from cart\n=. Calculate cost\nO. Checkout\nQ. Quit\n\n: ";
    bool is_running = true;

    ioopm_shop_t *shop = ioopm_create_shop();

    while (is_running)
    {
        char *input = ask_question_menu_option(menu_print);
        printf("\n");

        process_input(input, shop, &is_running);
        printf("\n");

        free(input);
    }

    ioopm_shop_destroy(shop);
    return 0;
}

void add_merchendice(ioopm_shop_t *shop)
{
    printf("Add merchandise\n");
    char *name = ask_question_string("Name: ");
    char *desc = ask_question_string("Description: ");
    int price = ask_question_int("Price: ");

    if (ioopm_shop_add_merch(shop, name, desc, price))
    {
        printf("\nMerchendice added.\n");
    }
    else
    {
        printf("\nFailed to add merchendice.\n");
    }

    free(name);
    free(desc);
}

void list_merchendice(ioopm_shop_t *shop)
{
    ioopm_list_t *merch = ioopm_shop_get_merch(shop);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(merch);

    if (ioopm_linked_list_size(merch) == 0)
    {
        printf("No merchendice in shop.\n");
    }
    else
    {
        printf("Merchendice:\n");
    }

    int counter = 0;
    bool success = true;
    while (success)
    {
        elem_t elem = ioopm_iterator_current(iter, &success);
        if (success)
        {
            printf("%s\n", (char *)elem.p);
            counter++;
        }

        if (counter == 20 && ioopm_iterator_has_next(iter))
        {
            char *input = ask_question("\nPress N to stop printing: ", everything_allowed, (convert_func *)strdup).string_value;
            if ((strlen(input) == 1) && char_eq_ignore_case(input, "n"))
            {
                free(input);
                break;
            }
            free(input);
            counter = 0;
        }
        ioopm_iterator_next(iter, &success);
    }

    ioopm_linked_list_destroy(merch);
    ioopm_iterator_destroy(iter);
}

void remove_merchandise(ioopm_shop_t *shop)
{
    printf("Remove merchandise\n");
    char *name = ask_question_string("Name: ");

    if (!user_confirmation())
    {
        printf("\nRemove aborted.\n");
        free(name);
        return;
    }

    if (ioopm_shop_remove_merch(shop, name))
    {
        printf("\nMerchandise removed successfully.\n");
    }
    else
    {
        printf("\nFailed to remove merchandise.\n");
    }
    free(name);
}

void edit_merchandise(ioopm_shop_t *shop)
{
    printf("Edit merchandise\n");
    char *old_name = ask_question_string("Merchandise to edit: ");
    char *new_name = ask_question_string("New name: ");
    char *desc = ask_question_string("New description: ");
    int price = ask_question_int("New price: ");

    if (!user_confirmation())
    {
        printf("\nEdit aborted.\n");
        free(old_name);
        free(new_name);
        free(desc);
        return;
    }

    if (ioopm_shop_edit_merchandise(shop, old_name, new_name, desc, price))
    {
        printf("\nMerchandise edited.\n");
    }
    else
    {
        printf("\nFailed to edit merchandise.\n");
    }

    free(old_name);
    free(new_name);
    free(desc);
}

void show_stock(ioopm_shop_t *shop)
{
    printf("Show stock\n");
    char *name = ask_question_string("Name: ");

    ioopm_list_t *stock = ioopm_shop_get_stock(shop, name);

    if (stock == NULL)
    {
        printf("\nCould not find merch.\n");
        free(name);
        return;
    }

    if (ioopm_linked_list_size(stock) == 0)
    {
        printf("\nMerchandise not in stock.\n");
    }
    else
    {
        printf("\nStock:\n");
    }

    ioopm_list_iterator_t *iter = ioopm_list_iterator(stock);

    bool success = true;
    while (success)
    {
        ioopm_shelf_t *shelf = ioopm_iterator_current(iter, &success).p;

        if (success)
        {
            printf("%s: %u\n", ioopm_shop_get_shelf_name(shelf), ioopm_shop_get_shelf_quantity(shelf));
        }

        ioopm_iterator_next(iter, &success);
    }

    free(name);
    ioopm_linked_list_destroy(stock);
    ioopm_iterator_destroy(iter);
}

bool is_shelf(char *str)
{
    // om strängen är empty eller mer/mindre än 3 tecken som behövs
    if (strlen(str) != 3)
    {
        return false;
    }
    // om formatet stämmer
    if (isalpha(str[0]) && isdigit(str[1]) && isdigit(str[2]))
    {
        return true;
    }
    return false;
}

void replenish_stock(ioopm_shop_t *shop)
{
    printf("Replenish stock\n");
    char *shelf_name = ask_question_string("Storage ID: ");

    if (!is_shelf(shelf_name))
    {
        printf("\nWrong shelf format\n"); 
        return;
    }

    char *merch_name = ask_question_string("Merchandise name: ");
    int quantity = ask_question_int("Quantity: ");

    if (quantity < 1)
    {
        printf("\nQuantity must be at least 1!\n");
    }
    else if (ioopm_shop_insert_stock(shop, merch_name, shelf_name, quantity))
    {
        printf("\nStock replenished.\n");
    }
    else
    {
        printf("\nFailed to replenish stock.\n");
    }

    free(shelf_name);
    free(merch_name);
}

void create_cart(ioopm_shop_t *shop)
{
    printf("Creating cart\n");
    unsigned int cart_id = ioopm_shop_create_cart(shop);
    printf("Cart ID: %u\n", cart_id);
}

void remove_cart(ioopm_shop_t *shop)
{
    printf("Remove cart\n");
    int cart_id = ask_question_int("Cart ID: ");

    printf("Removing cart: %u\n", cart_id);
    if (!user_confirmation())
    {
        printf("\nRemove aborted.\n");
        return;
    }

    if (ioopm_shop_remove_cart(shop, cart_id))
    {
        printf("\nCart removed successfully.\n");
    }
    else
    {
        printf("\nFailed to remove cart.\n");
    }
}

void add_to_cart(ioopm_shop_t *shop)
{
    printf("Add to cart\n");
    int cart_id = ask_question_int("Cart ID: ");
    char *merch_name = ask_question_string("Merchandise name: ");
    int quantity = ask_question_int("Quantity: ");

    if (quantity < 1)
    {
        printf("\nQuantity must be at least 1!\n");
    }
    else if (ioopm_shop_add_to_cart(shop, cart_id, merch_name, quantity))
    {
        printf("\nMerchandise added to cart.\n");
    }
    else
    {
        printf("\nFailed to add merchandise to cart.\n");
    }

    free(merch_name);
}

void remove_from_cart(ioopm_shop_t *shop)
{
    printf("Remove from cart\n");
    int cart_id = ask_question_int("Cart ID: ");
    char *merch_name = ask_question_string("Merchandise name: ");
    int quantity = ask_question_int("Quantity: ");

    if (ioopm_shop_remove_from_cart(shop, cart_id, merch_name, quantity))
    {
        printf("\nMerchandise removed from cart.\n");
    }
    else
    {
        printf("\nFailed to remove merchandise from cart.\n");
    }

    free(merch_name);
}

void calculate_cost(ioopm_shop_t *shop)
{
    printf("Calculate cost\n");
    int cart_id = ask_question_int("Cart ID: ");

    printf("Cart cost: %u\n", ioopm_shop_calculate_cost(shop, cart_id));
}

void checkout(ioopm_shop_t *shop)
{
    printf("Checkout\n");
    int cart_id = ask_question_int("Cart ID: ");

    if (ioopm_shop_checkout_cart(shop, cart_id))
    {
        printf("\nCart checkout successful.\n");
    }
    else
    {
        printf("\nFailed to checkout cart.\n");
    }
}

void quit_shop(bool *is_running)
{
    printf("Quitting shop\n");

    if (user_confirmation())
    {
        printf("\nQuitting.\n");
        *is_running = false;
    }
    else
    {
        printf("\nQuit aborted.\n");
    }
}

bool user_confirmation()
{
    char *confirmation = ask_question("\nAre you sure? (Y/y): ", everything_allowed, (convert_func *)strdup).string_value;
    if (char_eq_ignore_case(confirmation, "y"))
    {
        free(confirmation);
        return true;
    }
    free(confirmation);
    return false;
}