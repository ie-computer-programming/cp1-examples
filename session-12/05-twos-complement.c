/* 05 -- how negative numbers are stored: two's complement.
 *
 * One's complement:  -x is ~x          (flip every bit)   -> two zeros, +0 and -0
 * Two's complement:  -x is ~x + 1      (flip, then add 1) -> one zero
 *
 * Every machine you will ever use is two's complement, and C23 finally
 * requires it. The consequence worth remembering: the range is lopsided.
 * int8_t goes from -128 to +127, so -(-128) does not fit.
 */
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include "bits.h"

static void row(const char *label, int8_t v)
{
    printf("%-10s %5d   ", label, v);
    print_bits((uint8_t)v, 8);
    printf("\n");
}

int main(void)
{
    int8_t five = 5;

    row("5", five);
    row("~5", (int8_t)~five);              /* one's-complement "negative 5" */
    row("~5 + 1", (int8_t)(~five + 1));    /* two's complement: this IS -5  */
    row("-5", -five);
    printf("\n");
    row("0", 0);
    row("-1", -1);                         /* all ones */
    row("127", INT8_MAX);
    row("-128", INT8_MIN);                 /* 1 followed by zeros: no positive twin */
    printf("\n");

    /* Same lopsidedness at full width. abs(INT_MIN) is undefined behaviour,
     * because +2147483648 does not fit in an int. Widen first. */
    printf("INT_MIN          = %d\n", INT_MIN);
    printf("INT_MAX          = %d\n", INT_MAX);
    printf("-(long long)INT_MIN = %lld   (only representable in a wider type)\n",
           -(long long)INT_MIN);

    /* Right-shifting a negative value is implementation-defined. gcc and
     * clang copy the sign bit in ("arithmetic shift"); the standard does not
     * promise it. Python promises it. */
    printf("\n-8 >> 1          = %d   (gcc: arithmetic shift)\n", -8 >> 1);
    return 0;
}
