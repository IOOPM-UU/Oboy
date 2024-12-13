#ifndef __UTILS_H__
#define __UTILS_H__
// samma som pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> //för strlen
#include <ctype.h>  //för isdigit

typedef bool check_func(char *);

typedef union
{
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef answer_t convert_func(char *);

extern char *strdup(const char *);

int read_string(char *buf, int buf_siz);

bool is_number(char *str);

bool not_empty(char *str);

int ask_question_int(char *question);

char *ask_question_string(char *question);

int print(char *str);

int println(char *str);

answer_t ask_question(char *question, check_func *check, convert_func *convert);

#endif