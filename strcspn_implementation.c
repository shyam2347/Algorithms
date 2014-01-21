//
// Author Shyamsundar Parthasarathy
// Date Jan 15 2014
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ASCII_MAX 256

int my_strcspn(const char *str1, const char *str2)
{
  char key[ASCII_MAX];
  int index = 0;
  memset(key, 0, sizeof(key));
  while (str2[index])
    {
      key[str2[index]] = 1;
      index++;
    }
  index = 0;
  while (str1[index])
    {
      if (key[str1[index]] == 1)
        break;
      index++;
    }
  return index;
}
