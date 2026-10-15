/* 06 -- array vs linked list, measured.
 *
 * The textbook says: list insert at the front is O(1), array is O(n), so
 * lists win. The textbook leaves out the cache (Session 8, AoS vs SoA).
 * Walking a list means following a pointer to wherever malloc put the next
 * node -- and if that is far away, every step is a trip to main memory.
 *
 * Three walks over the same N numbers:
 *   array             contiguous
 *   list, in order    nodes allocated one after another -- usually adjacent
 *   list, shuffled    same nodes, linked in random order -- like a heap
 *                     that has seen a few thousand frees
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N_WALK   2000000
#define N_INSERT 50000

typedef struct node { int value; struct node *next; } Node;

static double secs(clock_t t0) { return (double)(clock() - t0) / CLOCKS_PER_SEC; }

static unsigned long long rng = 88172645463325252ull;
static unsigned long long xorshift(void)
{
    rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17;
    return rng;
}

static long long walk(const Node *head)
{
    long long sum = 0;
    for (; head != NULL; head = head->next) {
        sum += head->value;
    }
    return sum;
}

int main(void)
{
    int   *arr   = malloc(N_WALK * sizeof *arr);
    Node **nodes = malloc(N_WALK * sizeof *nodes);
    if (arr == NULL || nodes == NULL) {
        perror("malloc");
        return 1;
    }
    for (int i = 0; i < N_WALK; i++) {
        arr[i] = i % 100;
        nodes[i] = malloc(sizeof **nodes);
        if (nodes[i] == NULL) {
            perror("malloc");
            return 1;
        }
        nodes[i]->value = i % 100;
    }

    /* --- walk: array --- */
    clock_t t = clock();
    long long s_arr = 0;
    for (int i = 0; i < N_WALK; i++) {
        s_arr += arr[i];
    }
    double t_arr = secs(t);

    /* --- walk: list in allocation order --- */
    for (int i = 0; i < N_WALK; i++) {
        nodes[i]->next = (i + 1 < N_WALK) ? nodes[i + 1] : NULL;
    }
    t = clock();
    long long s_ord = walk(nodes[0]);
    double t_ord = secs(t);

    /* --- walk: same nodes, linked in shuffled order --- */
    for (int i = N_WALK - 1; i > 0; i--) {
        int j = (int)(xorshift() % (unsigned long long)(i + 1));
        Node *tmp = nodes[i]; nodes[i] = nodes[j]; nodes[j] = tmp;
    }
    for (int i = 0; i < N_WALK; i++) {
        nodes[i]->next = (i + 1 < N_WALK) ? nodes[i + 1] : NULL;
    }
    t = clock();
    long long s_shuf = walk(nodes[0]);
    double t_shuf = secs(t);

    printf("WALK %d elements, summing each\n", N_WALK);
    printf("  array            %.4f s   (sum %lld)\n", t_arr, s_arr);
    printf("  list, in order   %.4f s   (sum %lld)   %5.1fx the array\n", t_ord, s_ord, t_ord / t_arr);
    printf("  list, shuffled   %.4f s   (sum %lld)   %5.1fx the array\n\n", t_shuf, s_shuf, t_shuf / t_arr);

    /* --- insert at the front, N_INSERT times --- */
    int *front = malloc(N_INSERT * sizeof *front);
    if (front == NULL) {
        perror("malloc");
        return 1;
    }
    t = clock();
    for (int i = 0; i < N_INSERT; i++) {
        memmove(front + 1, front, (size_t)i * sizeof *front);   /* shift everything right */
        front[0] = i;
    }
    double t_ains = secs(t);

    Node *head = NULL;
    t = clock();
    for (int i = 0; i < N_INSERT; i++) {
        Node *n = malloc(sizeof *n);
        if (n == NULL) {
            perror("malloc");
            return 1;
        }
        n->value = i; n->next = head; head = n;
    }
    double t_lins = secs(t);

    printf("INSERT AT FRONT, %d times\n", N_INSERT);
    printf("  array (memmove)  %.4f s\n", t_ains);
    printf("  list  (push)     %.4f s   %5.1fx faster\n", t_lins, t_ains / t_lins);
    printf("\n(checksum %d %d)\n", front[0], head->value);

    while (head != NULL) { Node *nx = head->next; free(head); head = nx; }
    for (int i = 0; i < N_WALK; i++) {
        free(nodes[i]);
    }
    free(nodes); free(arr); free(front);
    return 0;
}
