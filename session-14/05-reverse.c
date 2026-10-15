/* 05 -- reversing a list in place: no allocation, three pointers.
 *
 *   prev   cur    next
 *   NULL   [1] -> [2] -> [3] -> NULL
 *
 * At each step: remember cur->next, point cur BACKWARDS at prev, then slide
 * all three one place to the right. When cur falls off the end, prev is the
 * new head. A classic interview question -- draw it on paper before coding.
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

static Node *list_reverse(Node *head)
{
    Node *prev = NULL;
    Node *cur  = head;
    while (cur != NULL) {
        Node *next = cur->next;   /* 1. remember where we were going */
        cur->next  = prev;        /* 2. turn this arrow around       */
        prev       = cur;         /* 3. step forward                 */
        cur        = next;
    }
    return prev;
}

static void list_print(const char *label, const Node *head)
{
    printf("%-10s", label);
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

    list_print("before:", head);
    head = list_reverse(head);
    list_print("after:", head);

    list_free(&head);
    return 0;
}
