#include <stdio.h>
#include <string.h>

int hash_function(char *str, int len)
{
    int h_count = 0;
    int i;
    for (i = 0; i < len; i++)
    {
        h_count += str[i];
    }
    return h_count;
}

char* rabinStr(char *str, char *substr)
{
    char *ptr;
    char *t_ptr;
    int substr_hash;
    int str_hash;
    int str_len = 0;
    int substr_len = 0;
    int i;

    t_ptr = str;
    while (*t_ptr)
    {
        str_len++;
        t_ptr++;
    }

    t_ptr = substr;
    while (*t_ptr)
    {
        substr_len++;
        t_ptr++;
    }

    substr_hash = hash_function(substr, substr_len);
    str_hash = hash_function(str, substr_len);

    ptr = str;
    for (i = 0; i < (str_len - substr_len + 1); i++)
    {
        if (str_hash == substr_hash)
        {
            return ptr+i;
        }
        str_hash -= str[i];
        str_hash += str[i + substr_len];
    }

    return NULL;
}

int main()
{
    char str[256] = "shyam";
    char substr[256] = "hy";

    char *ptr = NULL;

    ptr = rabinStr(str, substr);
    if (ptr)
    {
        printf("str %p\n", str);
        printf("Substring found %s\n", ptr);
    }

    printf("strstr %s\n", strstr(str, substr));

    return 0;
}
