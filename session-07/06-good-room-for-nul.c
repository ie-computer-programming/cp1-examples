/* GOOD: room for the terminator, and a checked copy.
 *
 * memcpy of n + 1 bytes copies the '\0' along with the characters, and says
 * exactly how many bytes move -- no scanning, no surprises.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Returns a block the CALLER must free(). NULL on failure. */
static char *duplicate(const char *s)
{
    size_t n = strlen(s);
    char *copy = malloc(n + 1);       /* + 1 for '\0' */
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, s, n + 1);
    return copy;
}

int main(void)
{
    char *c = duplicate("hello");
    if (c == NULL) {
        perror("duplicate");
        return 1;
    }

    printf("copy = %s (%zu bytes allocated)\n", c, strlen(c) + 1);
    free(c);
    return 0;
}
