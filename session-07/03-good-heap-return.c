/* GOOD: the heap outlives the function that allocated it.
 *
 * The rule this file demonstrates is OWNERSHIP: whoever receives the pointer
 * is now responsible for freeing it. Say so in the comment, every time --
 * there is no syntax in C to express it.
 */
#include <stdio.h>
#include <stdlib.h>

/* Returns a block the CALLER must free(). NULL on failure. */
static int *make_answer(void)
{
    int *answer = malloc(sizeof *answer);
    if (answer == NULL) {
        return NULL;
    }
    *answer = 42;
    return answer;       /* the block is on the heap; it survives */
}

int main(void)
{
    int *p = make_answer();
    if (p == NULL) {
        perror("make_answer");
        return 1;
    }

    printf("the answer is %d\n", *p);

    free(p);             /* we own it, so we free it */
    return 0;
}
