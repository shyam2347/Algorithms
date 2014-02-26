#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TEST_PARENT 0

typedef struct bst_node NODE;
struct bst_node
{
    int num;
    int height;
    NODE *left;
    NODE *right;
#if TEST_PARENT
    NODE *parent;
#endif
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
    ptr->height = 1;
    parent = root;
    while (parent)
    {
        if (num >= parent->num)
        {
            if (parent->right == NULL)
            {
                parent->right = ptr;
#if TEST_PARENT
                ptr->parent = parent;
#endif
                return ptr;
            }
            parent = parent->right;
        }
        else
        {
            if (parent->left == NULL)
            {
                parent->left = ptr;
#if TEST_PARENT
                ptr->parent = parent;
#endif
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
#if TEST_PARENT
    NODE *parent = ptr->parent;
    NODE *grandParent = parent->parent;
#else
    NODE *parent = getParent(ptr);
    NODE *grandParent = getParent(parent);
#endif

    parent->right = ptr->left;
#if TEST_PARENT
    if (ptr->left)
    {
        ptr->left->parent = parent;
    }
#endif
    if (parent->left == NULL && parent->right == NULL)
    {
        parent->height = 1;
    }
    else if (parent->left == NULL)
    {
        parent->height = parent->right->height + 1;
    }
    else if (parent->right == NULL)
    {
        parent->height = parent->left->height + 1;
    }
    else
    {
        if (parent->left->height >= parent->right->height)
        {
            parent->height = parent->left->height + 1;
        }
        else
        {
            parent->height = parent->right->height + 1;
        }
    }

    ptr->left = parent;
#if TEST_PARENT
    parent->parent = ptr;
#endif
    if (parent && (parent->height + 1 > ptr->height))
    {
        ptr->height = parent->height+1;
    }

#if TEST_PARENT
        ptr->parent = grandParent;
#endif
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
#if TEST_PARENT
    NODE *parent = ptr->parent;
    NODE *grandParent = parent->parent;
#else
    NODE *parent = getParent(ptr);
    NODE *grandParent = getParent(parent);
#endif

    parent->left = ptr->right;
#if TEST_PARENT
    if (ptr->right)
    {
        ptr->right->parent = parent;
    }
#endif
    if (parent->left == NULL && parent->right == NULL)
    {
        parent->height = 1;
    }
    else if (parent->left == NULL)
    {
        parent->height = parent->right->height + 1;
    }
    else if (parent->right == NULL)
    {
        parent->height = parent->left->height + 1;
    }
    else
    {
        if (parent->left->height >= parent->right->height)
        {
            parent->height = parent->left->height + 1;
        }
        else
        {
            parent->height = parent->right->height + 1;
        }
    }

    ptr->right = parent;
#if TEST_PARENT
    parent->parent = ptr;
#endif
    if (parent && (parent->height + 1 > ptr->height))
    {
        ptr->height = parent->height+1;
    } 
#if TEST_PARENT
        ptr->parent = grandParent;
#endif
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

#if TEST_PARENT
    parent = ptr->parent;
#else
    parent = getParent(ptr);
#endif
    while (parent)
    {
        if (ptr->height + 1 > parent->height)
        {
            parent->height = ptr->height+1;
        }
        lheight = (parent->left) ? parent->left->height : 0;
        rheight = (parent->right) ? parent->right->height : 0;
        if (abs(lheight - rheight) >= 2)
        {
            // Need rotation
            if (lheight - rheight == 2)
            {
                lheight = (parent->left->left) ? parent->left->left->height : 0;
                rheight = (parent->left->right) ? parent->left->right->height : 0;
                if (lheight - rheight == -1)
                {
                    rotate_left(parent->left->right);
                }
                rotate_right(parent->left);
            }
            else // lheight - rheight == -2
            {
                lheight = (parent->right->left) ? parent->right->left->height : 0;
                rheight = (parent->right->right) ? parent->right->right->height : 0;
                if (lheight - rheight == 1)
                {
                    rotate_right(parent->right->left);
                }
                rotate_left(parent->right);
            }
        }

        // After rotation
        ptr = parent;
#if TEST_PARENT
        parent = ptr->parent;
#else
        parent = getParent(ptr);
#endif
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

void delete_tree(NODE *ptr)
{
    if (ptr)
    {
        delete_tree(ptr->left);
        delete_tree(ptr->right);
        free(ptr);
    }
}

void printLevel(NODE *ptr, int level)
{
    if (!ptr)
        return;
    if (level == 1)
    {
        printf("%d:%d ", ptr->num, ptr->height);
    }
    else
    {
        printLevel(ptr->left, level-1);
        printLevel(ptr->right, level-1);
    }
}

void printLevels()
{
    int height = tree_height(root);
    int i;
    for (i = 1; i <= height; i++)
    {
        printLevel(root, i);
        printf("\n");
    }
}

int main()
{
    int i;
    root = NULL;
    NODES_COUNT = 0;
    clock_t start, end;

    start = clock();
    for (i = 100000; i > 0; i--)
    {
        insert(i);
    }
    end = clock();

    if (avl_search(1, root))
    printf("FOUND 1\n");

    if (avl_search_iter(1, root))
    printf("FOUND 1\n");
    
    //printTree(root);
    //printf("print iteratively\n");
    //printTree_iter();
    //printf("Print Level\n");
    //printLevels();
    printf("root height %d\n", root->height);
    printf("# of ticks %d, %f seconds\n", end-start, ((float)(end-start))/CLOCKS_PER_SEC);

    delete_tree(root);
    return 0;
}
