/* 04 -- removing a node, with a pointer to the link you are standing on.
 *
 * The textbook version tracks `prev` and special-cases the head, because
 * removing the head changes `head` while removing anything else changes
 * `prev->next`. Two cases, two chances to get one wrong.
 *
 * Walk a Node ** instead -- a pointer to "whichever link points at the
 * current node". At the start that is &head; afterwards it is &prev->next.
 * Either way you just overwrite *link. One case.
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

/* Removes the first node holding `value`. Returns 1 if found, 0 if not. */
static int list_remove(Node **head, int value)
{
    for (Node **link = head; *link != NULL; link = &(*link)->next) {
        if ((*link)->value == value) {
            Node *doomed = *link;
            *link = doomed->next;     /* unhook it -- head or not, same line */
            free(doomed);
            return 1;
        }
    }
    return 0;
}

static void list_print(const char *label, const Node *head)
{
    printf("%-18s", label);
    for (; head != NULL; head = head->next) {
        printf(" %d", head->value);
    }
    printf("\n");
}

static void list_free(Node **head)
{
    while (*head != NULL) {
        Node *next = (*head)->next;
        free(*head);
        *head = next;
    }
}

int main(void)
{
    Node *head = NULL;
    for (int i = 5; i >= 1; i--) {
        if (push_front(&head, i) != 0) {
            list_free(&head);
            return 1;
        }
    }

    list_print("start:", head);
    list_remove(&head, 1);  list_print("remove 1 (head):", head);
    list_remove(&head, 3);  list_print("remove 3 (middle):", head);
    list_remove(&head, 5);  list_print("remove 5 (tail):", head);
    printf("remove 9 (absent): %s\n", list_remove(&head, 9) ? "found" : "not found");

    list_free(&head);
    return 0;
}
