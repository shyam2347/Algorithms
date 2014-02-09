#include <stdio.h>
#include <string.h>

void swap(char *a, char *b)
{
    char t;
    t = *a;
    *a = *b;
    *b = t;
}

void permutate(char* str, int index)
{
    int i = 0;
    char lastChar = 0;
    if (index == strlen(str))
    { // We have a permutation so print it
        printf("%s\n", str);
        return;
    }
    for( i = index; i < strlen(str); i++ )
    {
        if (lastChar == str[i])
        {
            continue;
        }
        else
        {
            lastChar = str[i];
        }
        swap(&str[index], &str[i]); // It doesn't matter how you swap.
        permutate(str, index + 1);
        swap(&str[index], &str[i]);
    }
}

int main()
{
    char str[256] = "sss";
    permutate(str, 0);
}

