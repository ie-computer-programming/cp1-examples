/* 03-good -- read `next` BEFORE you free the node it lives in. */
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

/* Takes Node ** so it can also set the caller's head to NULL: free, then NULL. */
static void list_free(Node **head)
{
    Node *n = *head;
    while (n != NULL) {
        Node *next = n->next;    /* save it while n is still valid */
        free(n);
        n = next;
    }
    *head = NULL;
}

int main(void)
{
    Node *head = NULL;
    for (int i = 1; i <= 5; i++) {
        if (push_front(&head, i) != 0) {
            list_free(&head);
            return 1;
        }
    }

    list_free(&head);
    printf("list freed, head = %p\n", (void *)head);
    return 0;
}
