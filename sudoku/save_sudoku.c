#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_NODE_SIZE(x) x*x
#define ROW 1
#define COL 2

typedef struct elem
{
    int num;
    int *possible_nums;
}ELEM;

typedef struct sudoku_node
{
    ELEM **element;
    struct sudoku_node *left;
    struct sudoku_node *right;
    struct sudoku_node *up;
    struct sudoku_node *down;
}SUDOKU_NODE;

SUDOKU_NODE *root;
int sudoku_size;
int FOUND;

ELEM * insertElem(int size, int num)
{
    int index;
    ELEM *new_elem = (ELEM *) malloc(sizeof(ELEM));
    memset(new_elem, 0, sizeof(ELEM));
    new_elem->num = num;
    if (num == 0)
    {
        new_elem->possible_nums = (int *) malloc(sizeof(int) * size);
        for (index = 0; index < size; index++)
        {
            new_elem->possible_nums[index] = index+1;
        }
    }
    return new_elem;
}

int getPossibleCount(int *ptr, int *num)
{
    int i;
    int count = 0;
    int size = GET_NODE_SIZE(sudoku_size);
    for (i = 0; i < size; i++)
    {
        if (ptr[i] != 0)
        {
            count++;
            *num = ptr[i];
        }
    }
    return count;
}

void removeNumFromPossibleNums(ELEM **ptr, int num)
{
    int j;
    int i;
    int n;
    int size = GET_NODE_SIZE(sudoku_size);
    for (j = 0; j < size; j++)
    {
        if (ptr[j]->num == 0)
        {
            for (i = 0; i < size; i++)
            {
                if (ptr[j]->possible_nums[i] == num)
                {
                    ptr[j]->possible_nums[i] = 0;
                }
            }
            if (getPossibleCount(ptr[j]->possible_nums, size, &n) == 1)
            {
                FOUND = 1;
                ptr[j]->num = n;
                free(ptr[j]->possible_nums);
            }
        }
    }
}

void trimPossibleNums(ELEM **ptr)
{
    int i, j;
    int size = GET_NODE_SIZE(sudoku_size);
    for (i = 0; i < size; i++)
    {
        if (ptr[i]->num != 0)
        {
            removeNumFromPossibleNums(ptr, ptr[i]->num);
            for (j = 0; j < size; j++)
            {
                if (ptr[j]->num == 0)
                {
                    ptr[j]->possible_nums[ptr[i]->num - 1] = 0;
                }
            }
        }
    }
}

void insertNode(int size, char *line)
{
    char *str;
    int index = 0;
    int indexDown;
    SUDOKU_NODE *ptrNode;
    SUDOKU_NODE *ptrNodeVertical;
    SUDOKU_NODE *newNode;

    newNode = (SUDOKU_NODE *) malloc(sizeof(SUDOKU_NODE));
    memset(newNode, 0, sizeof(SUDOKU_NODE));
    str = strtok(line, " ");
    newNode->element = (ELEM **) malloc(sizeof(ELEM *) * size * size);
    while (str)
    {
        newNode->element[index] = insertElem(size*size, atoi(str));
        str = strtok(NULL, " ");
        index++;
    }

    // This will delete the present elements in the node from the possible numbers
    trimPossibleNums(newNode->element);

    if (!root)
    {
        root = newNode;
    }
    else
    {
        ptrNodeVertical = root;
        indexDown = 0;
        while (ptrNodeVertical && indexDown < size)
        {
            ptrNode = ptrNodeVertical;
            index = 0;
            while (ptrNode->right && index < size-1)
            {
                ptrNode = ptrNode->right;
                index++;
            }

            if (index != size-1)
            {
                ptrNode->right = newNode;
                newNode->left = ptrNode;
                break;
            }
            else if (!ptrNodeVertical->down)
            {
                ptrNodeVertical->down = newNode;
                newNode->up = ptrNodeVertical;
                break;
            }
            else
            {
                ptrNodeVertical = ptrNodeVertical->down;
            }
            indexDown++;
        }
    }
}

void printElements(ELEM **ptrElement, int size)
{
    int index;
    int j;
    size *= size;
    for (index = 0; index < size; index++)
    {
        printf("%d ", ptrElement[index]->num);
        if (ptrElement[index]->num == 0)
        {
            printf("--");
            for (j = 0; j < size; j++)
            {
                if (ptrElement[index]->possible_nums[j] != 0)
                    printf("%d ", ptrElement[index]->possible_nums[j]);
            }
            printf("--");
        }
    }
    printf("\n");
}

void printSudokuTree(SUDOKU_NODE *ptr, int size)
{
    int i,j;
    SUDOKU_NODE *ptr2;

    ptr2 = ptr;
    for (i = 0; i < size; i++)
    {
        ptr = ptr2;
        for (j = 0; j < size; j++)
        {
            if (ptr)
            {
                printElements(ptr->element, size);
                ptr = ptr->right;
            }
        }
        if (ptr2)
        {
            ptr2 = ptr2->down;
        }
    }
}

void clearNum(int num, int i, SUDOKU_NODE *ptr, int direction)
{
    if (ptr)
    {
        if (direction == ROW)
        {
        }
        else
        {
        }
    }
}

void populatePossibleNum(SUDOKU_NODE *ptr)
{
    int i;
    int size = GET_NODE_SIZE(sudoku_size);
    for (i = 0; i < size; i++)
    {
        if (ptr->element[i]->num != 0)
        {
            printf("num %d  \t", ptr->element[i]->num);
            clearNum(ptr->element[i]->num, i, ptr->left, ROW);
            clearNum(ptr->element[i]->num, i, ptr->right, ROW);
            clearNum(ptr->element[i]->num, i, ptr->up, COL);
            clearNum(ptr->element[i]->num, i, ptr->down, COL);
        }
    }
    printf("\n");
}

void trimSudoku()
{
    int i,j;
    SUDOKU_NODE *ptr1;
    SUDOKU_NODE *ptr2;
    while (1)
    {
        FOUND = 0;
        ptr1 = root;
        for (i = 0; i < sudoku_size; i++)
        {
            ptr2 = ptr1;
            for (j = 0; j < sudoku_size; j++)
            {
                if (ptr2)
                {
                    populatePossibleNum(ptr2);
                    ptr2 = ptr2->right;
                }
            }
            if (ptr1)
            {
                ptr1 = ptr1->down;
            }
        }
        if (FOUND == 0)
        {
            break;
        }
    }
}
    
void solveSudoku()
{
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char line[4096];
    int lineNum = 0;

    if (argc != 2)
    {
        printf("error - args not right\n");
        exit(1);
    }

    fp = fopen(argv[1], "rt");
    if (!fp)
    {
        printf("error - file not found\n");
        exit(1);
    }
    root = NULL;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (lineNum == 0)
        {
            sudoku_size = atoi(line);
        }
        else
        {
            insertNode(sudoku_size, line);
        }
        lineNum++;
    }
    fclose(fp);

    trimSudoku();

    printf("sudoku size %d\n", sudoku_size);
    printSudokuTree(root, sudoku_size);

    solveSudoku();
    return 0;
}
