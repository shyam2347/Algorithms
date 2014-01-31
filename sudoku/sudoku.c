#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_NODE_SIZE(x) x*x
#define ROW 1
#define COL 2
#define SUDOKU_SIZE 3

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
int FOUND;
int DEBUG = 0;

void printElements(SUDOKU_NODE *ptr);
void populatePossibleNum(SUDOKU_NODE *ptr);

int getPossibleCount(int ptr[], int *num)
{
    int i;
    int size = GET_NODE_SIZE(SUDOKU_SIZE);
    int count = 0;

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
                    FOUND = 1;
                    elem[i][j].num = n;
                }
            } // if num == 0
        } // for j
    } // for i
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

SUDOKU_NODE* getSudokuNode(int row, int col)
{
    int i;
    int j;
    SUDOKU_NODE *ptr1;
    SUDOKU_NODE *ptr2;

    ptr1 = root;
    for (i = 0; ptr1 && i < SUDOKU_SIZE; i++)
    {
        ptr2 = ptr1;
        for (j = 0; ptr2 && j < SUDOKU_SIZE; j++)
        {
            if (i == row && j == col)
            {
                return (ptr2 ? ptr2 : NULL);
            }
            ptr2 = ptr2->right;
        }
        ptr1 = ptr1->down;
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
                newNode->element[index / SUDOKU_SIZE][index % SUDOKU_SIZE].possible_nums[i] = i+1;
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

            while (ptrNode->right && index < SUDOKU_SIZE-1)
            {
                ptrNode = ptrNode->right;
                index++;
            }

            if (index != SUDOKU_SIZE-1)
            {
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
            if (DEBUG)
            {
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
    for (i = 0; ptr1 && i < SUDOKU_SIZE; i++)
    {
        ptr2 = ptr1;
        for (j = 0; ptr2 && j < SUDOKU_SIZE; j++)
        {
            printElements(ptr2);
            ptr2 = ptr2->right;
        }
        ptr1 = ptr1->down;
    }
}

int removeNumFromElement(ELEM *e, int num)
{
    int size = GET_NODE_SIZE(SUDOKU_SIZE);
    int i;
    int n;
    int count = 0;

    for (i = 0; i < size; i++)
    {
        if (e->num == 0 && e->possible_nums[i] == num)
        {
            count++;
            e->possible_nums[i] = 0;
        }
    }

    if (getPossibleCount(e->possible_nums, &n) == 1)
    {
        FOUND = 1;
        e->num = n;
    }
    return count;
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
    } // if (ptr)
}

SUDOKU_NODE* getLeftNode(SUDOKU_NODE *ptr, int k)
{
    int i;
    for (i = 0; ptr && i < k; i++)
    {
        ptr = ptr->left;
    }
    return (ptr ? ptr->left : NULL);
}

SUDOKU_NODE* getRightNode(SUDOKU_NODE *ptr, int k)
{
    int i;
    for (i = 0; ptr && i < k; i++)
    {
        ptr = ptr->right;
    }
    return (ptr ? ptr->right : NULL);
}

SUDOKU_NODE* getUpNode(SUDOKU_NODE *ptr, int k)
{
    int i;
    for (i = 0; ptr && i < k; i++)
    {
        ptr = ptr->up;
    }
    return (ptr ? ptr->up : NULL);
}

SUDOKU_NODE* getDownNode(SUDOKU_NODE *ptr, int k)
{
    int i;
    for (i = 0; ptr && i < k; i++)
    {
        ptr = ptr->down;
    }
    return (ptr ? ptr->down : NULL);
}

int getPossibleNumCountInRow(SUDOKU_NODE *ptr, int row, int k)
{
    int i;
    int count = 0;;
    if (!ptr)
    {
        return 0;
    }

    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        if (ptr->element[row][i].num == 0 && 
            ptr->element[row][i].possible_nums[k] != 0)
        {
            count++;
        } 
    }
    return count;
}


// 
// Fix a row, 
// check column and then keep moving left and find if there is anything unique
// then keep moving right and find if there is anything unique
void removeUniqueFromHorizontal(SUDOKU_NODE *ptr)
{
    int k;
    int i;
    int j;
    int l;
    int checked = 0;
    int count;
    int size = GET_NODE_SIZE(SUDOKU_SIZE);

    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if (ptr && (ptr->element[i][j].num == 0))
            {
                for (k = 0; k < size; k++)
                {
                    checked = getPossibleNumCountInRow(ptr, i, k);
                    if (checked == 1)
                    {
                        printf("Inside check 1 %d\n", k);
                        for (l = 0; l < SUDOKU_SIZE-1; l++)
                        {
                            checked += getPossibleNumCountInRow(getLeftNode(ptr, l), i, k);
                            checked += getPossibleNumCountInRow(getRightNode(ptr, l), i, k);
                        }
                        if (checked == 1)
                        {
                            printf("checked %d\n", k);
                            count = removeNumFromElement(&(ptr->element[i][j]), k+1);
                            if (count  > 0)
                            {
                                ptr->element[i][j].num = k+1;
                            }
                            printElements(ptr);
                        }
                    }
                }
            } // for j
        } // for i
    }

#if 0
    if (DEBUG)
    {
        printf("end of remove unique from horizontal\n");
        printElements(ptr);
    }
#endif
}

void removeUniqueFromNode(SUDOKU_NODE *ptr)
{
    int i;
    int j;
    int k;
    int checked;
    int count;
    int size = GET_NODE_SIZE(SUDOKU_SIZE);
    
    if (DEBUG)
    {
        //printf("remove unique from node\n");
        //printElements(ptr);
    }

    for (k = 0; k < size; k++)
    {
        checked = 0;
        for (i = 0; i < SUDOKU_SIZE; i++)
        {
            for (j = 0; j < SUDOKU_SIZE; j++)
            {
                if (ptr && (ptr->element[i][j].num == 0) && 
                    (ptr->element[i][j].possible_nums[k] != 0))
                {
                    checked++;
                }
            }
        }
        if (checked == 1)
        {
            if (DEBUG)
            {
                //printf("checked %d\n", k);
                //printElements(ptr);
            }
            for (i = 0; i < SUDOKU_SIZE; i++)
            {
                for (j = 0; j < SUDOKU_SIZE; j++)
                {
                    if (ptr->element[i][j].num == 0)
                    {
                        count = removeNumFromElement(&(ptr->element[i][j]), k+1);
                        if (count > 0)
                        {
                            ptr->element[i][j].num = k+1;
                        }
                    }
                }
            }
        }
    }
}

void populatePossibleNum(SUDOKU_NODE *ptr)
{
    int i;
    int j;
    int k;
    int size = GET_NODE_SIZE(SUDOKU_SIZE);
    for (i = 0; i < SUDOKU_SIZE; i++)
    {
        for (j = 0; j < SUDOKU_SIZE; j++)
        {
            if (ptr && ptr->element[i][j].num != 0)
            {
                for (k = 0; k < SUDOKU_SIZE-1; k++)
                {
                    clearNum(ptr->element[i][j].num, i, j, getLeftNode(ptr, k), ROW);
                    clearNum(ptr->element[i][j].num, i, j, getRightNode(ptr, k), ROW);
                    clearNum(ptr->element[i][j].num, i, j, getUpNode(ptr, k), COL);
                    clearNum(ptr->element[i][j].num, i, j, getDownNode(ptr, k), COL);
                }
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
        for (i = 0; ptr1 && i < SUDOKU_SIZE; i++)
        {
            ptr2 = ptr1;
            for (j = 0; ptr2 && j < SUDOKU_SIZE; j++)
            {
                populatePossibleNum(ptr2);
                ptr2 = ptr2->right;
            }
            ptr1 = ptr1->down;
        }

        //if (DEBUG)
        {
            printf("After first round\n");
            printSudokuTree(root);
        }
        if (FOUND)
        {
            continue;
        }

        // find unique in node and trim
        ptr1 = root;
        for (i = 0; ptr1 && i < SUDOKU_SIZE; i++)
        {
            ptr2 = ptr1;
            for (j = 0; ptr2 && j < SUDOKU_SIZE; j++)
            {
                removeUniqueFromNode(ptr2);
                trimPossibleNums(ptr2->element);
                ptr2 = ptr2->right;
            }
            ptr1 = ptr1->down;
        }

        //if (DEBUG)
        {
            printf("After second round\n");
            printSudokuTree(root);
        }
        if (FOUND)
        {
            continue;
        }

        // find unique in line and trim
        ptr1 = root;
        for (i = 0; ptr1 && i < SUDOKU_SIZE; i++)
        {
            ptr2 = ptr1;
            for (j = 0; ptr2 && j < SUDOKU_SIZE; j++)
            {
                // 2 types
                // 1) horizontal line computation
                if (j == 1)
                {
                    printElements(ptr2);
                }
                removeUniqueFromHorizontal(ptr2);
                if (j == 1)
                {
                    printElements(ptr2);
                }
                trimPossibleNums(ptr2->element);
                // 2) vertical line computation
                //removeUniqueFromVertical(ptr2);
                ptr2 = ptr2->right;
            }
            ptr1 = ptr1->down;
        }

        //if (DEBUG)
        {
            printf("After third round\n");
            printSudokuTree(root);
        }

        if (FOUND == 0)
        {
            break;
        }
    }
}
    
int main(int argc, char *argv[])
{
    FILE *fp;
    char line[4096];
    int lineNum = 0;
    int sudoku_size;

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

    return 0;
}
