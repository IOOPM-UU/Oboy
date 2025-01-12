#pragma once
#include <stdbool.h>

/// Compares two elements and returns true if they are equal
typedef union lib_elem lib_elem_t;
typedef bool lib_eq_function(lib_elem_t a, lib_elem_t b);
typedef bool lib_predicate(lib_elem_t key, lib_elem_t value, void *extra);
typedef void lib_apply_function(lib_elem_t key, lib_elem_t *value, void *extra);


union lib_elem // TODO: Ska den vara här? Användaren kan nog ej göra en eq_func utan att veta struktur på elem
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
#define lib_int_elem(x) (lib_elem_t) { .i=(x) }

/// @brief Macro to create an element from an unsigned integer value.
/// @param x The unsigned integer value to be wrapped in the elem_t union.
/// @return An elem_t union containing the unsigned integer.
#define lib_u_elem(x) (lib_elem_t) { .u=(x) }

/// @brief Macro to create an element from a void pointer
/// @param x The void pointer to be wrapped in the elem_t union.
/// @return An elem_t union containing the string.
#define lib_ptr_elem(x) (lib_elem_t) { .p= (x) }
