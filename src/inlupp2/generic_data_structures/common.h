#pragma once
#include <stdbool.h>

/// Compares two elements and returns true if they are equal
typedef union elem elem_t;
typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef bool ioopm_predicate(elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);


union elem // TODO: Ska den vara här? Användaren kan nog ej göra en eq_func utan att veta struktur på elem
{
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
};

/// @brief Macro to create an element from an integer value.
/// @param x The integer value to be wrapped in the elem_t union.
/// @return An elem_t union containing the integer.
#define int_elem(x) (elem_t) { .i=(x) }

/// @brief Macro to create an element from an unsigned integer value.
/// @param x The unsigned integer value to be wrapped in the elem_t union.
/// @return An elem_t union containing the unsigned integer.
#define u_elem(x) (elem_t) { .u=(x) }

/// @brief Macro to create an element from a void pointer
/// @param x The void pointer to be wrapped in the elem_t union.
/// @return An elem_t union containing the string.
#define ptr_elem(x) (elem_t) { .p= (x) }
