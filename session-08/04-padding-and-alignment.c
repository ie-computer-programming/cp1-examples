/* 04 -- sizeof is not the sum of the parts.
 *
 * The compiler inserts PADDING so that every member sits at an address that
 * is a multiple of its own size (its alignment). The hardware needs this;
 * reading an int from a non-multiple-of-4 address is slow or illegal.
 *
 * Consequence: the ORDER you declare fields in changes how much memory the
 * struct takes. Same three fields, two layouts, different sizes.
 */
#include <stdio.h>
#include <stddef.h>   /* offsetof */

struct wasteful {
    char   a;    /* 1 byte  + 7 padding  */
    double b;    /* 8 bytes              */
    char   c;    /* 1 byte  + 7 padding  */
};

struct tidy {
    double b;    /* 8 bytes */
    char   a;    /* 1 byte  */
    char   c;    /* 1 byte  + 6 padding at the end */
};

int main(void)
{
    printf("sizeof(char)   = %zu\n", sizeof(char));
    printf("sizeof(double) = %zu\n", sizeof(double));
    printf("sum of members = %zu\n\n", sizeof(char) * 2 + sizeof(double));

    printf("struct wasteful: sizeof = %zu\n", sizeof(struct wasteful));
    printf("  offsetof a = %zu\n", offsetof(struct wasteful, a));
    printf("  offsetof b = %zu\n", offsetof(struct wasteful, b));
    printf("  offsetof c = %zu\n\n", offsetof(struct wasteful, c));

    printf("struct tidy:     sizeof = %zu\n", sizeof(struct tidy));
    printf("  offsetof b = %zu\n", offsetof(struct tidy, b));
    printf("  offsetof a = %zu\n", offsetof(struct tidy, a));
    printf("  offsetof c = %zu\n\n", offsetof(struct tidy, c));

    printf("same fields, %zu bytes saved by declaring the big ones first\n",
           sizeof(struct wasteful) - sizeof(struct tidy));

    /* Because of the padding bytes, memcmp() on two structs can report a
     * difference even when every member is equal: the gaps are uninitialised.
     * Compare members, never raw bytes. */
    return 0;
}
