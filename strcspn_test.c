//
// Author Shyamsundar Parthasarathy
// Date Jan 15 2014
//

#include <stdio.h>
#include <string.h>
#include "strcspn_implementation.c"

void t_one()
{
    char str[] = "fcba73";
    char keys[] = "1234567890";

    int i;
    i = my_strcspn(str, keys);
    printf("The first number in str is at position %d. \n", i+1);
}

void t_two()
{
    char str[] = "fcbaas";
    char keys[] = "1234567890";

    int i;
    i = my_strcspn(str, keys);
    printf("strlen %d\n", strlen(str));
    printf("The first number in str is at position %d. \n", i+1);
}

int main()
{
    t_one();
    t_two();
    return 0;
}
