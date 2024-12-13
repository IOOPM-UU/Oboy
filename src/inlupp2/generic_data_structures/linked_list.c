#include "linked_list.h"
#include "iterator.h"
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include <stddef.h>

#define null_elem \
    (elem_t) { 0 }

typedef bool ioopm_int_predicate(int index, elem_t value, void *extra);
typedef void ioopm_apply_int_function(int index, elem_t *value, void *extra);
typedef struct list ioopm_list_t;
typedef struct link link_t;
typedef struct iter ioopm_list_iterator_t;

struct list
{
    size_t size;
    link_t *first;
    link_t *last;
    ioopm_eq_function *eq_func;
};

struct link
{
    elem_t value;
    link_t *previous;
    link_t *next;
};

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}

static link_t *link_create(elem_t value, link_t *previous, link_t *next)
{
    link_t *link = calloc(1, sizeof(link_t));
    link->previous = previous;
    link->next = next;
    link->value = value;
    return link;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function *eq_func)
{
    ioopm_list_t *result = calloc(1, sizeof(ioopm_list_t));
    result->size = 0;
    result->first = result->last = link_create(null_elem, NULL, NULL);
    result->eq_func = eq_func;
    return result;
}


ioopm_list_t *ioopm_linked_list_duplicate(ioopm_list_t *list)
{
    ioopm_list_t *dup = ioopm_linked_list_create(list->eq_func);

    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

    bool success = true;
    while (success)
    {
        elem_t elem = ioopm_iterator_current(iter, &success);

        if (success)
        {
            ioopm_linked_list_append(dup, elem);
        }

        ioopm_iterator_next(iter, &success);
    }

    ioopm_iterator_destroy(iter);
    return dup;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    ioopm_linked_list_clear(list);
    free(list->first);
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    link_t *new_link = link_create(value, list->last, NULL);

    list->last->next = new_link;
    list->last = new_link;
    list->size++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    link_t *dummy = list->first;
    link_t *new_link = link_create(value, dummy, dummy->next);

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

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
    if (index >= list->size)
    {
        ioopm_linked_list_append(list, value);
        return;
    }

    if (index == 0)
    {
        ioopm_linked_list_prepend(list, value);
        return;
    }

    link_t *current = list->first;
    for (int i = 0; i < index && current->next != NULL; i++)
    {
        current = current->next;
    }

    link_t *after_current = current->next;
    link_t *new_link = link_create(value, current, after_current);
    current->next = new_link;

    if (new_link->next == NULL)
    {
        list->last = new_link;
    }

    list->size++;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index, bool *success)
{
    if (index < 0 || index >= list->size)
    {
        *success = false;
        return null_elem;
    }
    link_t *dummy = list->first;
    if (index == 0)
    {
        link_t *to_be_removed = dummy->next;
        elem_t value_removed = to_be_removed->value;

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
    link_t *current_link = dummy;
    for (int i = 0; i < index; i++)
    {
        current_link = current_link->next;
    }

    link_t *to_be_removed = current_link->next;
    elem_t value_removed = to_be_removed->value;

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

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index, bool *success)
{
    if (index < 0 || index >= list->size)
    {
        *success = false;
        return null_elem;
    }

    link_t *current_link = list->first->next;

    for (int i = 0; i < index; i++)
    {
        current_link = current_link->next;
    }

    *success = true;
    return current_link->value;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate *prop, void *extra)
{
    link_t *cursor = list->first->next;
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

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate *prop, void *extra)
{
    link_t *current_entry = list->first->next;

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

ioopm_list_option_t ioopm_linked_list_lookup(ioopm_list_t *list, elem_t element)
{
    link_t *cursor = list->first->next;

    while (cursor != NULL)
    {
        if (list->eq_func(cursor->value, element))
        {
            return (ioopm_list_option_t){.value = cursor->value, .success = true};
        }
        cursor = cursor->next;
    }
    return (ioopm_list_option_t){.success = false};
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
    link_t *cursor = list->first->next;

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

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function *fun, void *extra)
{
    link_t *cursor = list->first->next;
    int counter = 0;

    while (cursor != NULL)
    {
        fun(counter, &cursor->value, extra);
        cursor = cursor->next;
        counter++;
    }
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    link_t *dummy = list->first;
    link_t *current_link = dummy->next;

    while (current_link != NULL)
    {
        link_t *next_link = current_link->next;

        free(current_link);

        current_link = next_link;
    }

    dummy->next = NULL;
    list->last = dummy;
    list->size = 0;
}

/* ITERATOR */

struct iter
{
    link_t *current;
    ioopm_list_t *list;
};

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));

    iter->list = list;
    ioopm_iterator_reset(iter);

    return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    if (iter->current == NULL)
    {
        return false;
    }
    return iter->current->next != NULL;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter, bool *success)
{
    if (iter->current == NULL)
    {
        *success = false;
        return null_elem;
    }
    if (ioopm_iterator_has_next(iter))
    {
        iter->current = iter->current->next;
        *success = true;
        return iter->current->value;
    }
    *success = false;
    return null_elem;
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter, bool *success)
{
    if (iter->current == NULL)
    {
        *success = false;
        return null_elem;
    }

    if (ioopm_linked_list_is_empty(iter->list))
    {
        *success = false;
        return null_elem;
    }

    link_t *to_remove = iter->current;
    elem_t removed_value = to_remove->value;

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

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
    link_t *new_link;
    if (ioopm_linked_list_is_empty(iter->list))
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
        ioopm_iterator_reset(iter);
    }

    iter->list->size++;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = iter->list->first->next;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter, bool *success)
{
    if (iter->current == NULL)
    {
        *success = false;
        return null_elem;
    }
    if (ioopm_linked_list_is_empty(iter->list))
    {
        *success = false;
        return null_elem;
    }
    *success = true;
    return iter->current->value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}
