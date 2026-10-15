/* 02-good -- to change the caller's pointer, pass the ADDRESS of the pointer.
 *
 * It is exactly swap() from Session 6, one level up:
 *     to change an int,     pass an int *
 *     to change a Node *,   pass a Node **
 *
 * Returns 0 on success, -1 if malloc failed (the list is left unchanged).
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
    n->next  = *head;         /* the old first node */
    *head    = n;             /* the CALLER's head now points here */
    return 0;
}

static void list_free(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    Node *head = NULL;

    for (int i = 1; i <= 3; i++) {
        if (push_front(&head, i) != 0) {
            perror("push_front");
            list_free(head);
            return 1;
        }
    }

    printf("list:");
    for (const Node *n = head; n != NULL; n = n->next) {
        printf(" %d", n->value);
    }
    printf("   <-- pushed 1, 2, 3 at the FRONT\n");

    list_free(head);
    return 0;
}
