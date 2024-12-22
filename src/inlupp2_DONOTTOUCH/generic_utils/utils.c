extern char *strdup(const char *);
#include <stdlib.h>
#include <string.h> //för strlen
#include <ctype.h>  //för isdigit
#include <stdio.h>
#include "utils.h"

bool not_empty(char *str)
{
    return strlen(str) > 0;
}

bool is_number(char *str)
{
    if (!not_empty(str))
    {
        return false;
    }
    int length = strlen(str); // tar ut längden på strängen
    int i = 0;
    if (str[i] == '-')
    {
        if (length == 1)
        {
            return false; // ett minus är inte ett tal
        }
        i = 1; // så skippar man första tecknet när vi kör isdigit
    }

    for (; i < length; ++i)
    {
        if (!isdigit((unsigned char)str[i]))
        { // om strängen inte är ett tal
            return false;
        }
    }
    return true;
}

int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_number, (convert_func *)atoi);
    return answer.int_value; // svaret som ett heltal
}

int read_string(char *buf, int buf_siz)
{
    char c;
    int count = 0; // variabel för string-length
    do
    {
        c = getchar();  // tar nästa tecken
        buf[count] = c; // lägger in karaktären i arrayen
        if (c != '\n')
        {
            count++; // incrementerar counten
        }
    } while (c != '\n' && count < buf_siz); // så länge strängen inte tar slut

    buf[count] = '\0'; // null-terminate string
    return count;
}

char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func *)strdup).string_value;
}

int print(char *str)
{ // utan radbrytning
    int i = 0;
    while (str[i] != '\0')
    {
        putchar(str[i]);
        i++;
    }
    return 0;
}

int println(char *str)
{ // med radbrytning
    print(str);
    putchar('\n');
    return 0;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
    int buf_siz = 255;
    char buf[buf_siz];
    bool valid_input = false;

    do
    {
        printf("%s", question);
        read_string(buf, buf_siz);
        // check if input is valid with check function
        valid_input = check(buf);
        if (!valid_input)
        {
            printf("\nInvalid input, try again\n\n");
        }
    } while (valid_input == false);

    // convert the input and return
    return convert(buf);
}
