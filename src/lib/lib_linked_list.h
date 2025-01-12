#pragma once

#include "lib_common.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct lib_list lib_list_t;
typedef struct lib_iter lib_list_iterator_t;
typedef struct lib_list_option lib_list_option_t;
typedef bool lib_int_predicate(int index, lib_elem_t value, void *extra);
typedef void lib_apply_int_function(int index, lib_elem_t *value, void *extra);

struct lib_list_option
{
    bool success;
    lib_elem_t value;
};

/// @brief Creates a new empty list
/// @return an empty linked list
lib_list_t *lib_linked_list_create(lib_eq_function *eqfn);

lib_list_t *lib_linked_list_duplicate(lib_list_t *list);

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void lib_linked_list_destroy(lib_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
void lib_linked_list_append(lib_list_t *list, lib_elem_t value);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
void lib_linked_list_prepend(lib_list_t *list,lib_elem_t value);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted
void lib_linked_list_insert(lib_list_t *list, int index,lib_elem_t value);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list
/// @param index the position in the list
/// @param success a stack variable pointer to indicate success or failure
/// @return the value removed
lib_elem_t lib_linked_list_remove(lib_list_t *list, int index, bool *success);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param success a stack variable pointer to indicate success or failure
/// @return the value at the given position
lib_elem_t lib_linked_list_get(lib_list_t *list, int index, bool *success);

/// @brief Returns element if found in list
/// @param list the linked list
/// @param element the element sought
/// @return list_option with value and bool indicating success
lib_list_option_t lib_linked_list_lookup(lib_list_t *list, lib_elem_t element);

/// @brief Test if an element is in the list
/// @param list the linked list
/// @param element the element sought
/// @return true if element is in the list, else false
lib_list_option_t lib_linked_list_contains(lib_list_t *list,lib_elem_t element);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t lib_linked_list_size(lib_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool lib_linked_list_is_empty(lib_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list the linked list
void lib_linked_list_clear(lib_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
bool lib_linked_list_all(lib_list_t *list, lib_int_predicate *prop, void *extra);

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool lib_linked_list_any(lib_list_t *list, lib_int_predicate *prop, void *extra);

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of fun
void lib_linked_list_apply_to_all(lib_list_t *list, lib_apply_int_function *fun, void *extra);

/// @brief Create an iterator for a given list
/// @param list the list to be iterated over
/// @return an iteration positioned at the start of list
lib_list_iterator_t *lib_list_iterator(lib_list_t *list);