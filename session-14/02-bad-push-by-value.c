/* 02-bad -- push_front that cannot change the caller's head.
 *
 * Session 6 and Session 8 again: C passes everything by value. Here the
 * "everything" is a POINTER. push_front receives a copy of `head`, points
 * the copy at the new node, and returns. The caller's head never moves.
 *
 * Result: the list stays empty, and every node allocated is leaked.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct node { int value; struct node *next; } Node;

static void push_front(Node *head, int value)
{
    Node *n = malloc(sizeof *n);
    if (n == NULL) {
        return;
    }
    n->value = value;
    n->next  = head;
    head = n;                 /* changes the LOCAL copy only */
}

int main(void)
{
    Node *head = NULL;

    for (int i = 1; i <= 3; i++) {
        push_front(head, i);
    }

    printf("head = %p\n", (void *)head);
    printf("list:");
    for (Node *n = head; n != NULL; n = n->next) {
        printf(" %d", n->value);
    }
    printf("   <-- empty\n");
    return 0;
}
