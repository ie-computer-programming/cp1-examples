/* BAD: the off-by-one that every C programmer writes once.
 *
 * strlen("hello") is 5. A copy of "hello" needs SIX bytes: five characters
 * and the '\0'. This is the same rule HW1's cp1_copy_bounded enforced -- the
 * terminator has to fit inside the buffer.
 *
 * Watch it get caught:
 *     ./build/06-bad-off-by-one-asan
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *duplicate(const char *s)
{
    char *copy = malloc(strlen(s));   /* <-- missing the + 1 */
    if (copy == NULL) {
        return NULL;
    }
    strcpy(copy, s);                  /* writes strlen(s) + 1 bytes */
    return copy;
}

int main(void)
{
    char *c = duplicate("hello");
    if (c == NULL) {
        perror("duplicate");
        return 1;
    }

    printf("copy = %s\n", c);   /* often "works" -- that is the danger */
    free(c);
    return 0;
}
