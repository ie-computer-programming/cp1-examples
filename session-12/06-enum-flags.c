/* 06 -- enums as flags: many booleans packed into one integer.
 *
 * Each enumerator is a distinct power of two, so each owns one bit. Combine
 * with |, test with &, remove with & ~. This is exactly how Unix file
 * permissions (chmod 755), open() flags (O_RDONLY | O_CREAT) and every
 * graphics API's option bits work. Python's equivalent is enum.Flag.
 */
#include <stdio.h>

typedef enum {
    PERM_NONE  = 0,
    PERM_READ  = 1u << 2,     /* 4 */
    PERM_WRITE = 1u << 1,     /* 2 */
    PERM_EXEC  = 1u << 0,     /* 1 */
} Perm;

static void print_perm(const char *who, unsigned p)
{
    printf("%-8s %c%c%c  (%u)\n", who,
           (p & PERM_READ)  ? 'r' : '-',
           (p & PERM_WRITE) ? 'w' : '-',
           (p & PERM_EXEC)  ? 'x' : '-',
           p);
}

int main(void)
{
    unsigned owner = PERM_READ | PERM_WRITE | PERM_EXEC;   /* 7 */
    unsigned group = PERM_READ | PERM_EXEC;                /* 5 */
    unsigned other = PERM_READ;                            /* 4 */

    print_perm("owner", owner);
    print_perm("group", group);
    print_perm("other", other);
    printf("as chmod: %u%u%u\n\n", owner, group, other);

    other |= PERM_WRITE;       /* grant */
    print_perm("other+w", other);
    other &= ~PERM_WRITE;      /* revoke */
    print_perm("other-w", other);

    /* "has ALL of these" vs "has ANY of these" -- a common confusion. */
    unsigned want = PERM_READ | PERM_WRITE;
    printf("\ngroup has ALL of rw? %s\n", ((group & want) == want) ? "yes" : "no");
    printf("group has ANY of rw? %s\n", ((group & want) != 0)    ? "yes" : "no");

    /* The variables are `unsigned`, not `Perm`: a combination such as 5 is
     * not one of the named enumerators. */
    return 0;
}
