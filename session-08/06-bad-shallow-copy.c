/* 06-bad -- struct assignment copies the POINTER, not what it points at.
 *
 * `b = a` copies every member. If a member is a `char *`, the copy is the
 * ADDRESS. You now have two structs that believe they own the same string:
 *   - writing through one changes the other
 *   - freeing both is a double free
 *
 * This is the same trap as Python's `b = a.copy()` on a dict of lists, except
 * there is no exception waiting for you at the end -- just a crash, or not.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;   /* owned */
    int   id;
} Student;

static Student student_make(const char *name, int id)
{
    Student s = {.name = malloc(strlen(name) + 1), .id = id};
    if (s.name != NULL) {
        memcpy(s.name, name, strlen(name) + 1);
    }
    return s;
}

int main(void)
{
    Student a = student_make("Ada", 1815);
    if (a.name == NULL) {
        perror("malloc");
        return 1;
    }

    Student b = a;            /* shallow copy: b.name == a.name */
    b.id = 9999;

    printf("a.name = %p  b.name = %p\n", (void *)a.name, (void *)b.name);

    b.name[0] = 'X';          /* meant to touch b only */
    printf("a.name = %s   <-- changed through b\n", a.name);

    free(b.name);
    free(a.name);             /* double free: same address, second time */

    printf("still here\n");
    return 0;
}
