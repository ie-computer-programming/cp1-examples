/* 07 -- a union is ONE block of memory with several names and types.
 *
 * struct: all members at once, side by side. sizeof = sum + padding.
 * union:  one member at a time, all at the same address. sizeof = largest.
 *
 * Writing one member overwrites the others. Reading a member you did not
 * write gives you the raw bytes of whatever is actually in there,
 * reinterpreted -- which is occasionally the point, and usually a bug.
 */
#include <stdio.h>
#include <stdint.h>

union value {
    int32_t  as_int;
    float    as_float;
    uint8_t  as_bytes[4];
};

struct pair {
    int32_t a;
    float   b;
};

int main(void)
{
    printf("sizeof(union value) = %zu   (largest member)\n", sizeof(union value));
    printf("sizeof(struct pair) = %zu   (all members)\n\n", sizeof(struct pair));

    union value v;
    v.as_int = 1;

    printf("wrote  as_int   = %d\n", v.as_int);
    printf("bytes           = %02x %02x %02x %02x\n",
           v.as_bytes[0], v.as_bytes[1], v.as_bytes[2], v.as_bytes[3]);
    printf("read   as_float = %g   <-- same bits, different meaning\n\n",
           (double)v.as_float);

    v.as_float = 1.0f;   /* overwrites all four bytes */

    printf("wrote  as_float = %g\n", (double)v.as_float);
    printf("bytes           = %02x %02x %02x %02x\n",
           v.as_bytes[0], v.as_bytes[1], v.as_bytes[2], v.as_bytes[3]);
    printf("read   as_int   = %d   <-- the IEEE-754 bit pattern as an integer\n",
           v.as_int);

    /* The byte order above is little-endian on x86 and Apple Silicon:
     * the least significant byte comes first. */
    return 0;
}
