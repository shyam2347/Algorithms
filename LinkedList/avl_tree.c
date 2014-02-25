#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct bst_node NODE;
struct bst_node
{
    int num;
    NODE *left;
    NODE *right;
};

typedef struct queue QUEUE;

struct queue
{
    NODE *bst_ptr;
    QUEUE *next;
};

NODE *root;
int NODES_COUNT;

// Search in avl tree
// Returns 1 if num found, 0 when not found.
// recursive solution
int avl_search(int num, NODE *ptr)
{
    if (ptr)
    {
        if (ptr->num == num)
            return 1;
        else if (ptr->num > num)
            return avl_search(num, ptr->left);
        else
            return avl_search(num, ptr->right);
    }
    return 0;
}

// Search iteratively
int avl_search_iter(int num, NODE *ptr)
{
    while (ptr)
    {
        if (ptr->num == num)
            return 1;
        else if (ptr->num > num)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }
    return 0;
}

NODE* bst_insert(int num)
{
    NODE *parent;
    NODE *ptr = (NODE *) malloc(sizeof(NODE));
    memset(ptr, 0, sizeof(NODE));
    ptr->num = num;
    parent = root;
    while (parent)
    {
        if (num >= parent->num)
        {
            if (parent->right == NULL)
            {
                parent->right = ptr;
                return ptr;
            }
            parent = parent->right;
        }
        else
        {
            if (parent->left == NULL)
            {
                parent->left = ptr;
                return ptr;
            }
            parent = parent->left;
        }
    }
    root = ptr;
    return ptr;
}

int avltree_height()
{
    return ceil(log2(NODES_COUNT));
}

int tree_height(NODE *ptr)
{
    int count = 0;
    int lcount = 0;
    if (ptr)
    {
        lcount = tree_height(ptr->left)+1;
        count = tree_height(ptr->right)+1;
    }
    if (lcount > count)
        return lcount;
    else
        return count;
}

void freeQueue(QUEUE **q)
{
    QUEUE *ptr;
    while (*q)
    {
        ptr = *q;
        *q = (*q)->next;
        free(ptr);
    }
    *q = NULL;
}

void insertQueue(QUEUE **q, NODE *n)
{
    QUEUE *ptr;
    QUEUE *newQ;

    if (n)
    {
        newQ = (QUEUE *) malloc(sizeof(QUEUE));
        memset(newQ, 0, sizeof(QUEUE));
        newQ->bst_ptr = n;
        if (*q = NULL)
        {
            *q = newQ;
        }
        else
        {
            ptr = *q;
            while (ptr->next)
            {
                ptr = ptr->next;
            }
            ptr->next = newQ;
        }
    }
}

// Using 2 Queue approach to find height of the tree iteratively using BFS
int tree_height_iter(NODE *ptr)
{
    int height = 0;
    QUEUE *q1 = NULL;
    QUEUE *q2 = NULL;
    QUEUE *ptrQ;

    insertQueue(&q1, ptr);
    while (q1 != NULL || q2 != NULL)
    {
        if (q1 == NULL)
        {
            ptrQ = q2;
            while(ptrQ)
            {
                insertQueue(&q1, ptrQ->bst_ptr->left);
                insertQueue(&q1, ptrQ->bst_ptr->right);
                ptrQ = ptrQ->next;
            }
            freeQueue(&q2);
        }
        else
        {
            ptrQ = q1;
            while(ptrQ)
            {
                insertQueue(&q2, ptrQ->bst_ptr->left);
                insertQueue(&q2, ptrQ->bst_ptr->right);
                ptrQ = ptrQ->next;
            }
            freeQueue(&q1);
        }
        height++;
    }
    return height;
}

NODE* getParent(NODE *ptr)
{
    NODE *parent = root;

    while (parent)
    {
        if (parent->left == ptr || parent->right == ptr)
            return parent;
        else if (ptr->num >= parent->num)
            parent = parent->right;
        else
            parent = parent->left;
        
    }
    return NULL;
}

void rotate_left(NODE *ptr)
{
    NODE *parent = getParent(ptr);
    NODE *grandParent = getParent(parent);

    parent->right = ptr->left;
    ptr->left = parent;
    if (parent == root)
    {
        root = ptr;
    }
    else
    {
        if (grandParent->left == parent)
        {
            grandParent->left = ptr;
        }
        else
        {
            grandParent->right = ptr;
        }
    }
}

void rotate_right(NODE *ptr)
{
    NODE *parent = getParent(ptr);
    NODE *grandParent = getParent(parent);

    parent->left = ptr->right;
    ptr->right = parent;
    if (parent == root)
    {
        root = ptr;
    }
    else
    {
        if (grandParent->left == parent)
        {
            grandParent->left = ptr;
        }
        else
        {
            grandParent->right = ptr;
        }
    }
}

void printTree(NODE *ptr);

void insert(int num)
{
    NODE *ptr;
    NODE *parent;
    int lheight;
    int rheight;
    ptr = bst_insert(num);
    NODES_COUNT++;

    parent = getParent(ptr);
    while (parent)
    {
        lheight = tree_height(parent->left);
        rheight = tree_height(parent->right);
        if (abs(lheight - rheight) > 1)
        {
            // Need rotation
            if (lheight - rheight == 2)
            {
                lheight = tree_height(parent->left->left);
                rheight = tree_height(parent->left->right);
                if (lheight - rheight == -1)
                {
                    rotate_left(parent->left->right);
                }
                rotate_right(parent->left);
            }
            else // lheight - rheight == -2
            {
                lheight = tree_height(parent->right->left);
                rheight = tree_height(parent->right->right);
                if (lheight - rheight == 1)
                {
                    rotate_right(parent->right->left);
                }
                rotate_left(parent->right);
            }
        }

        // After rotation
        ptr = parent;
        parent = getParent(ptr);
    }
}


// Prints tree recurisvely
void printTree(NODE *ptr)
{
    if (ptr)
    {
        printf("%d\n", ptr->num);
        printTree(ptr->left);
        printTree(ptr->right);
    }
}

// prints tree iteratively
void printTree_iter()
{
    NODE *stack[avltree_height()];
    NODE *ptr;
    int index = 0;
    stack[index++] = root;
    while (index != 0)
    {
        ptr = stack[--index];
        printf("%d\n", ptr->num);
        if (ptr->right)
            stack[index++] = ptr->right;
        if (ptr->left)
            stack[index++] = ptr->left;
    }
}

int main()
{
    root = NULL;
    NODES_COUNT = 0;

    insert(1);
    insert(2);
    insert(3);
    insert(4);
    insert(5);
    insert(6);
    insert(7);

    if (avl_search(1, root))
    printf("FOUND 1\n");

    if (avl_search_iter(1, root))
    printf("FOUND 1\n");
    
    printTree(root);
    printf("print iteratively\n");
    printTree_iter();
    return 0;
}
