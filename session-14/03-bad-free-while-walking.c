/* 03-bad -- freeing a list with the obvious loop.
 *
 *     for (Node *n = head; n != NULL; n = n->next) free(n);
 *
 * After free(n), the loop's step reads n->next -- from the block it just
 * gave back. On glibc that aborts: free() stores its own bookkeeping inside
 * the freed block, right where `next` was, so the loop follows a garbage
 * pointer into the next free(). gcc already warns: -Wuse-after-free.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct node { int value; struct node *next; } Node;

static int push_front(Node **head, int value)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL) {
        return -1;
    }
    n->value = value;
    n->next  = *head;
    *head    = n;
    return 0;
}

int main(void)
{
    Node *head = NULL;
    for (int i = 1; i <= 5; i++) {
        if (push_front(&head, i) != 0) {
            return 1;
        }
    }

    for (Node *n = head; n != NULL; n = n->next) {   /* n->next AFTER free(n) */
        free(n);
    }

    printf("list freed -- apparently\n");
    return 0;
}
