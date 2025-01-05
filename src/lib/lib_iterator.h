#pragma once
#include <stdbool.h>
#include "lib_common.h"

typedef struct lib_iter lib_list_iterator_t;

/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if there is at least one more element
bool lib_iterator_has_next(lib_list_iterator_t *iter);

/// @brief Step the iterator forward one step
/// @param iter the iterator
/// @param success pointer to a stack variable, indicating success
/// @return the next element
lib_elem_t lib_iterator_next(lib_list_iterator_t *iter, bool *success);

/// NOTE: Resets iterator if last element in list is removed
/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @param success pointer to a stack variable, indicating success
/// @return the removed element
lib_elem_t lib_iterator_remove(lib_list_iterator_t *iter, bool *success);

/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
void lib_iterator_insert(lib_list_iterator_t *iter, lib_elem_t element);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
void lib_iterator_reset(lib_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @param success pointer to a stack variable, indicating success
/// @return the current element
lib_elem_t lib_iterator_current(lib_list_iterator_t *iter, bool *success);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void lib_iterator_destroy(lib_list_iterator_t *iter);