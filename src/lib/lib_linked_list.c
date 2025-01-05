#include "lib_linked_list.h"
#include "lib_iterator.h"
#include <stdbool.h>
#include <stdlib.h>
#include "lib_common.h"
#include <stddef.h>

#define null_elem \
    (lib_elem_t) { 0 }

typedef bool lib_int_predicate(int index, lib_elem_t value, void *extra);
typedef void lib_apply_int_function(int index, lib_elem_t *value, void *extra);
typedef struct lib_list lib_list_t;
typedef struct lib_link lib_link_t;
typedef struct lib_iter lib_list_iterator_t;

struct lib_list
{
    size_t size;
    lib_link_t *first;
    lib_link_t *last;
    lib_eq_function *eq_func;
};

struct lib_link
{
    lib_elem_t value;
    lib_link_t *previous;
    lib_link_t *next;
};

bool lib_linked_list_is_empty(lib_list_t *list)
{
    return list->size == 0;
}

static lib_link_t *link_create(lib_elem_t value, lib_link_t *previous, lib_link_t *next)
{
    lib_link_t *link = calloc(1, sizeof(lib_link_t));
    link->previous = previous;
    link->next = next;
    link->value = value;
    return link;
}

lib_list_t *lib_linked_list_create(lib_eq_function *eq_func)
{
    lib_list_t *result = calloc(1, sizeof(lib_list_t));
    result->size = 0;
    result->first = result->last = link_create(null_elem, NULL, NULL);
    result->eq_func = eq_func;
    return result;
}


lib_list_t *lib_linked_list_duplicate(lib_list_t *list)
{
    lib_list_t *dup = lib_linked_list_create(list->eq_func);

    lib_list_iterator_t *iter = lib_list_iterator(list);

    bool success = true;
    while (success)
    {
        lib_elem_t elem = lib_iterator_current(iter, &success);

        if (success)
        {
            lib_linked_list_append(dup, elem);
        }

        lib_iterator_next(iter, &success);
    }

    lib_iterator_destroy(iter);
    return dup;
}

void lib_linked_list_destroy(lib_list_t *list)
{
    lib_linked_list_clear(list);
    free(list->first);
    free(list);
}

void lib_linked_list_append(lib_list_t *list, lib_elem_t value)
{
    lib_link_t *new_link = link_create(value, list->last, NULL);

    list->last->next = new_link;
    list->last = new_link;
    list->size++;
}

void lib_linked_list_prepend(lib_list_t *list, lib_elem_t value)
{
    lib_link_t *dummy = list->first;
    lib_link_t *new_link = link_create(value, dummy, dummy->next);

    if (dummy->next != NULL)
    {
        dummy->next->previous = new_link;
    }

    dummy->next = new_link;

    if (list->last == dummy)
    {
        list->last = new_link;
    }

    list->size++;
}

void lib_linked_list_insert(lib_list_t *list, int index, lib_elem_t value)
{
    if (index >= list->size)
    {
        lib_linked_list_append(list, value);
        return;
    }

    if (index == 0)
    {
        lib_linked_list_prepend(list, value);
        return;
    }

   lib_link_t *current = list->first;
    for (int i = 0; i < index && current->next != NULL; i++)
    {
        current = current->next;
    }

   lib_link_t *after_current = current->next;
   lib_link_t *new_link = link_create(value, current, after_current);
    current->next = new_link;

    if (new_link->next == NULL)
    {
        list->last = new_link;
    }

    list->size++;
}

lib_elem_t lib_linked_list_remove(lib_list_t *list, int index, bool *success)
{
    if (index < 0 || index >= list->size)
    {
        *success = false;
        return null_elem;
    }
   lib_link_t *dummy = list->first;
    if (index == 0)
    {
       lib_link_t *to_be_removed = dummy->next;
        lib_elem_t value_removed = to_be_removed->value;

        dummy->next = to_be_removed->next;

        if (list->last == to_be_removed)
        {
            list->last = dummy;
        }

        free(to_be_removed);
        list->size--;
        *success = true;
        return value_removed;
    }
   lib_link_t *current_link = dummy;
    for (int i = 0; i < index; i++)
    {
        current_link = current_link->next;
    }

   lib_link_t *to_be_removed = current_link->next;
    lib_elem_t value_removed = to_be_removed->value;

    current_link->next = to_be_removed->next;

    if (to_be_removed == list->last)
    {
        list->last = current_link;
    }
    free(to_be_removed);
    list->size--;
    *success = true;
    return value_removed;
}

lib_elem_t lib_linked_list_get(lib_list_t *list, int index, bool *success)
{
    if (index < 0 || index >= list->size)
    {
        *success = false;
        return null_elem;
    }

   lib_link_t *current_link = list->first->next;

    for (int i = 0; i < index; i++)
    {
        current_link = current_link->next;
    }

    *success = true;
    return current_link->value;
}

size_t lib_linked_list_size(lib_list_t *list)
{
    return list->size;
}

bool lib_linked_list_all(lib_list_t *list, lib_int_predicate *prop, void *extra)
{
   lib_link_t *cursor = list->first->next;
    int counter = 0;

    while (cursor != NULL)
    {
        if (!prop(counter, cursor->value, extra))
        {
            return false;
        }
        cursor = cursor->next;
        counter++;
    }
    return true;
}

bool lib_linked_list_any(lib_list_t *list, lib_int_predicate *prop, void *extra)
{
   lib_link_t *current_entry = list->first->next;

    for (int i = 0; i < list->size; i++)
    {
        if (prop(i, current_entry->value, extra))
        {
            return true;
        }
        current_entry = current_entry->next;
    }
    return false;
}

lib_list_option_t lib_linked_list_lookup(lib_list_t *list, lib_elem_t element)
{
   lib_link_t *cursor = list->first->next;

    while (cursor != NULL)
    {
        if (list->eq_func(cursor->value, element))
        {
            return (lib_list_option_t){.value = cursor->value, .success = true};
        }
        cursor = cursor->next;
    }
    return (lib_list_option_t){.success = false};
}

bool lib_linked_list_contains(lib_list_t *list, lib_elem_t element)
{
   lib_link_t *cursor = list->first->next;

    while (cursor != NULL)
    {
        if (list->eq_func(cursor->value, element))
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

void lib_linked_list_apply_to_all(lib_list_t *list, lib_apply_int_function *fun, void *extra)
{
   lib_link_t *cursor = list->first->next;
    int counter = 0;

    while (cursor != NULL)
    {
        fun(counter, &cursor->value, extra);
        cursor = cursor->next;
        counter++;
    }
}

void lib_linked_list_clear(lib_list_t *list)
{
   lib_link_t *dummy = list->first;
   lib_link_t *current_link = dummy->next;

    while (current_link != NULL)
    {
       lib_link_t *next_link = current_link->next;

        free(current_link);

        current_link = next_link;
    }

    dummy->next = NULL;
    list->last = dummy;
    list->size = 0;
}

/* ITERATOR */

struct lib_iter
{
   lib_link_t *current;
    lib_list_t *list;
};

lib_list_iterator_t *lib_list_iterator(lib_list_t *list)
{
    lib_list_iterator_t *iter = calloc(1, sizeof(lib_list_iterator_t));

    iter->list = list;
    lib_iterator_reset(iter);

    return iter;
}

bool lib_iterator_has_next(lib_list_iterator_t *iter)
{
    if (iter->current == NULL)
    {
        return false;
    }
    return iter->current->next != NULL;
}

lib_elem_t lib_iterator_next(lib_list_iterator_t *iter, bool *success)
{
    if (iter->current == NULL)
    {
        *success = false;
        return null_elem;
    }
    if (lib_iterator_has_next(iter))
    {
        iter->current = iter->current->next;
        *success = true;
        return iter->current->value;
    }
    *success = false;
    return null_elem;
}

lib_elem_t lib_iterator_remove(lib_list_iterator_t *iter, bool *success)
{
    if (iter->current == NULL)
    {
        *success = false;
        return null_elem;
    }

    if (lib_linked_list_is_empty(iter->list))
    {
        *success = false;
        return null_elem;
    }

   lib_link_t *to_remove = iter->current;
    lib_elem_t removed_value = to_remove->value;

    iter->current->previous->next = to_remove->next;

    if (to_remove == iter->list->last)
    {
        iter->list->last = iter->current->previous;
    }

    iter->current = iter->current->next;

    free(to_remove);
    iter->list->size--;

    *success = true;
    return removed_value;
}

void lib_iterator_insert(lib_list_iterator_t *iter, lib_elem_t element)
{
   lib_link_t *new_link;
    if (lib_linked_list_is_empty(iter->list))
    {
        new_link = link_create(element, iter->list->first, NULL);
        iter->list->first->next = new_link;
        iter->list->last = new_link;
    }
    else
    {
        new_link = link_create(element, iter->current->previous, iter->current);
        iter->current->previous->next = new_link;

        iter->current->previous = new_link;
    }

    iter->current = new_link;

    if (iter->list->size == 0)
    {
        lib_iterator_reset(iter);
    }

    iter->list->size++;
}

void lib_iterator_reset(lib_list_iterator_t *iter)
{
    iter->current = iter->list->first->next;
}

lib_elem_t lib_iterator_current(lib_list_iterator_t *iter, bool *success)
{
    if (iter->current == NULL)
    {
        *success = false;
        return null_elem;
    }
    if (lib_linked_list_is_empty(iter->list))
    {
        *success = false;
        return null_elem;
    }
    *success = true;
    return iter->current->value;
}

void lib_iterator_destroy(lib_list_iterator_t *iter)
{
    free(iter);
}
