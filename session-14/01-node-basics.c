/* 01 -- a linked list: nodes on the heap, each pointing at the next.
 *
 * Python's list is NOT this. It is a growable array (Session 7's realloc
 * example) -- one contiguous block. A linked list is many small blocks,
 * scattered wherever malloc put them, held together only by pointers.
 *
 *   head -> [ 1 | *-]-> [ 2 | *-]-> [ 3 | NULL]
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int          value;
    struct node *next;      /* the tag is why this line compiles: `Node` does not exist yet */
} Node;

/* Caller frees. NULL on failure. */
static Node *node_new(int value, Node *next)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL) {
        return NULL;
    }
    n->value = value;
    n->next  = next;
    return n;
}

int main(void)
{
    /* Built back to front, so each node can point at one that already exists. */
    Node *third  = node_new(3, NULL);
    Node *second = node_new(2, third);
    Node *head   = node_new(1, second);
    if (third == NULL || second == NULL || head == NULL) {
        perror("malloc");
        return 1;
    }

    /* The one loop every linked-list function is built from. */
    for (const Node *n = head; n != NULL; n = n->next) {
        printf("node at %p: value = %d, next = %p\n",
               (const void *)n, n->value, (const void *)n->next);
    }

    free(head);
    free(second);
    free(third);
    return 0;
}
