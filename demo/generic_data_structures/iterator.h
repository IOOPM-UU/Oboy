#pragma once
#include <stdbool.h>
#include "common.h"

typedef struct iter ioopm_list_iterator_t;

/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if there is at least one more element
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step
/// @param iter the iterator
/// @param success pointer to a stack variable, indicating success
/// @return the next element
elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter, bool *success);

/// NOTE: Resets iterator if last element in list is removed
/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @param success pointer to a stack variable, indicating success
/// @return the removed element
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter, bool *success);

/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @param success pointer to a stack variable, indicating success
/// @return the current element
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter, bool *success);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);