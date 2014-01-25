#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_NODE_SIZE(x) x*x
#define ROW 1
#define COL 2
#define SUDOKU_SIZE 2

typedef struct elem
{
    int num;
    int possible_nums[SUDOKU_SIZE*SUDOKU_SIZE];
}ELEM;

typedef struct sudoku_node
{
    ELEM element[SUDOKU_SIZE][SUDOKU_SIZE];
    struct sudoku_node *left;
    struct sudoku_node *right;
    struct sudoku_node *up;
    struct sudoku_node *down;
}SUDOKU_NODE;

SUDOKU_NODE *root;
int sudoku_size;
int FOUND;
int DEBUG = 0;

void printElements(SUDOKU_NODE *ptr);

int getPossibleCount(int ptr[], int *num)
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

void removeNumFromNode(ELEM elem[SUDOKU_SIZE][SUDOKU_SIZE], int num)
{
    int i;
    int j;
    int k;
    int n;
    int size = GET_NODE_SIZE(SUDOKU_SIZE);

    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if (elem[i][j].num == 0)
            {
                for (k = 0; k < size; k++)
                {
                    if (elem[i][j].possible_nums[k] == num)
                    {
                        elem[i][j].possible_nums[k] = 0;
                    }
                }
                if (getPossibleCount(elem[i][j].possible_nums, &n) == 1)
                {
                    elem[i][j].num = n;
                }
            }
        }
    }
}

void trimPossibleNums(ELEM elem[SUDOKU_SIZE][SUDOKU_SIZE])
{
    int i, j ,k;
    int size = GET_NODE_SIZE(SUDOKU_SIZE);

    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if (elem[i][j].num != 0)
            {
                removeNumFromNode(elem, elem[i][j].num);
            }
        }
    }
}

SUDOKU_NODE * getSudokuNode(int row, int col)
{
    int i;
    int j;
    SUDOKU_NODE *ptr1;
    SUDOKU_NODE *ptr2;

    ptr1 = root;
    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        ptr2 = ptr1;
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if (i == row && j == col)
            {
                if (ptr2)
                    return ptr2;
                else
                    return NULL;
            }
            if (ptr2)
            {
                ptr2 = ptr2->right;
            }
        }
        if (ptr1)
        {
            ptr1 = ptr1->down;
        }
    }
    return NULL;
}

void insertNode(char *line)
{
    char *str;
    int index;
    int i;
    int indexDown;
    int size;
    SUDOKU_NODE *ptrNode;
    SUDOKU_NODE *ptrNodeVertical;
    SUDOKU_NODE *newNode;
    SUDOKU_NODE *tmp;

    newNode = (SUDOKU_NODE *) malloc(sizeof(SUDOKU_NODE));
    memset(newNode, 0, sizeof(SUDOKU_NODE));

    size = GET_NODE_SIZE(SUDOKU_SIZE);

    str = strtok(line, " ");
    index = 0;
    while (str)
    {
        newNode->element[index / SUDOKU_SIZE][index % SUDOKU_SIZE].num = atoi(str);
        if (0 == atoi(str))
        {
            for (i = 0; i < size; i++)
            {
                newNode->element[index / sudoku_size][index % sudoku_size].possible_nums[i] = i+1;
            }
        }
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
        while (ptrNodeVertical && indexDown < SUDOKU_SIZE)
        {
            ptrNode = ptrNodeVertical;
            index = 0;
            if (DEBUG)
            {
                printf("inside while\n");
            }

            while (ptrNode->right && index < SUDOKU_SIZE-1)
            {
                ptrNode = ptrNode->right;
                index++;
            }

            if (index != SUDOKU_SIZE-1)
            {
                if (DEBUG)
                    printf("Insert right\n");

                ptrNode->right = newNode;
                newNode->left = ptrNode;
                if (indexDown != 0)
                {
                    tmp = getSudokuNode(indexDown-1, index+1);
                    if (tmp)
                    {
                    newNode->up = tmp;
                    tmp->down = newNode;
                    }
                }
                break;
            }
            else if (!ptrNodeVertical->down)
            {
                if (DEBUG)
                    printf("insert down\n");
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

void printElements(SUDOKU_NODE *ptr)
{
    int index;
    int i;
    int j;
    int k;
    int size = GET_NODE_SIZE (SUDOKU_SIZE);

    if (!ptr)
    {
        return;
    }

    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            printf("%d ", ptr->element[i][j].num);
            if (ptr->element[i][j].num == 0)
            {
                printf(" -- ");
                for (k = 0; k < size; k++)
                {
                    if (ptr->element[i][j].possible_nums[k] != 0)
                    {
                        printf("%d ", ptr->element[i][j].possible_nums[k]);
                    }
                }
                printf(" -- ");
            }
        }
    }
    printf("\n");
}

void printSudokuTree()
{
    int i,j;
    SUDOKU_NODE *ptr1;
    SUDOKU_NODE *ptr2;

    printf("\n\nPrintingTree\n\n");
    ptr1 = root;
    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        ptr2 = ptr1;
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if (ptr2)
            {
                printElements(ptr2);
                ptr2 = ptr2->right;
            }
        }
        if (ptr1)
        {
            ptr1 = ptr1->down;
        }
    }
}

void removeNumFromElement(ELEM *e, int num)
{
    int size = GET_NODE_SIZE(SUDOKU_SIZE);
    int i;
    int n;

    if (DEBUG)
    {
        printf("Remove num from elements %d\n", num);
    }
    for (i = 0; i < size; i++)
    {
        if (e->possible_nums[i] == num)
        {
            e->possible_nums[i] = 0;
        }
    }

    if (getPossibleCount(e->possible_nums, &n) == 1)
    {
        FOUND = 1;
        e->num = n;
    }
}

void clearNum(int num, int row, int col, SUDOKU_NODE *ptr, int direction)
{
    int i;
    if (ptr)
    {
        if (direction == ROW)
        {
            for (i = 0; i < SUDOKU_SIZE; i++)
            {
                if (ptr->element[row][i].num == 0)
                {
                    removeNumFromElement(&(ptr->element[row][i]), num);
                    trimPossibleNums(ptr->element);
                }
            } 
        }
        else
        {
            for (i = 0; i < SUDOKU_SIZE; i++)
            {
                if (ptr->element[i][col].num == 0)
                {
                    removeNumFromElement(&(ptr->element[i][col]), num);
                    trimPossibleNums(ptr->element);
                }
            }
        }
    }
}

void populatePossibleNum(SUDOKU_NODE *ptr)
{
    int i;
    int j;
    int size = GET_NODE_SIZE(sudoku_size);
    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if (ptr->element[i][j].num != 0)
            {
                clearNum(ptr->element[i][j].num, i, j, ptr->left, ROW);
                clearNum(ptr->element[i][j].num, i, j, ptr->right, ROW);
                clearNum(ptr->element[i][j].num, i, j, ptr->up, COL);
                clearNum(ptr->element[i][j].num, i, j, ptr->down, COL);
            }
        }
    }
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

    if (argc < 2)
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
    if ((argc >= 3) && (strcmp(argv[2], "DBG") == 0))
    {
        DEBUG = 1;
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
            insertNode(line);
        }
        lineNum++;
    }
    fclose(fp);

    printf("sudoku size %d\n", sudoku_size);
    printSudokuTree(root);

    trimSudoku();

    printSudokuTree(root);

    solveSudoku();
    return 0;
}
