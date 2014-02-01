//
// Test program to verify if SUDOKU solver is bug free
// Program orignially written for codeeval competition, adapting it
// to automated test.
// Author shyam (shyam2347@gmail.com)
//
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


int checkRow(int *array, int m)
{
    int i;
    int j;
    for (i = 0; i < m; i++)
    {
        for (j = i+1; j < m; j++)
        {
            if (array[i] == array[j])
                return 0;
        } 
    }
    return 1;
}

int checkColumn (int *array, int m)
{
    int i;
    int j;
    for (i = 0; i < m; i++)
    {
        for (j = i+1; j < m; j++)
        {
            if (array[i*m] == array[j*m])
                return 0;
        }
    }
    return 1;
}

int checkSmallArray (int *array, int m, int dim)
{
    int i, j;
    for (i = 0; i < m; i++)
    {
        for (j = i+1; j < m; j++)
        {
            if (array[((i/dim)*m) + i%dim] == array[((j/dim)*m) + j%dim])
                return 0;
        }    
    }
    return 1;
}

int mSquareCheck(int *array, int m)
{
    int dim = (int)sqrt(m);
    int i, j;
    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
            if (checkSmallArray(&(array[i*dim + (j*dim*m)]), m, dim) == 0)
                return 0;
        }
    }
    return 1;
}

const char* checkSudoku(int *array, int m)
{
    int i, j;
    int *tmpArray;
    int arrSize = m*m;

    // Zero Check
    for (i = 0; i < m; i++)
    {
        if (array[i] == 0)
        {
            return "False";
        }
    }

    // Row Check
    tmpArray = array;
    for (i = 0; i < m; i++)
    {
        if (checkRow(tmpArray, m) == 0)
            return "False"; 
        tmpArray += m;
    }

    // Column Check
    tmpArray = array;
    for (i = 0; i < m; i++)
    {
        if (checkColumn(tmpArray, m) == 0)
            return "False";
        tmpArray++;
    }

    // m Square check
    tmpArray = array;
    if (mSquareCheck(tmpArray, m) == 0)
        return "False";

    return "True";
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *str;
    int lineNum = 0;
    int index;
    int *array;
    int arrayIndex;
    int square;
    if (argc < 2)
    {
        printf("Argument not present\n");
        exit(1);
    }
    fp = fopen(argv[1], "r");
    if (NULL == fp)
    {
        printf("File not present\n");
        exit(1);
    }
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (lineNum != 0)
        {
            printf("\n");
        }
        lineNum++;
        index = 0;
        str = strtok(line, ";");
        if (!str)
        {
            continue;
        }
        square = atoi(str);
        array = (int *) malloc(square * square * sizeof(int));
        if (array == NULL)
        {
            printf("malloc failed\n");
            exit(1);
        }
        arrayIndex = 0;
        str = strtok(NULL,",");
        while (str)
        {
            array[arrayIndex++] = atoi(str);
            str = strtok(NULL,",");
        }
        printf("%s", checkSudoku(array, square));
        free(array);
    }
    fclose(fp);
    return 0;
}
