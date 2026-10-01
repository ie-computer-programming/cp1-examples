/* 07 -- # makes a string, ## glues tokens together.
 *
 *   #x      "stringify": turn the ARGUMENT TEXT into a string literal
 *   a ## b  "paste":     join two tokens into one identifier
 *
 * Both operate on text, before the compiler exists. That leads to the single
 * most surprising rule in the preprocessor: an argument to # or ## is NOT
 * macro-expanded first. You need a second macro to force the expansion.
 */
#include <stdio.h>

#define STR(x)    #x          /* one level: stringifies the literal text */
#define XSTR(x)   STR(x)      /* two levels: expands x first, THEN stringifies */

#define CAT(a, b) a ## b

#define VERSION 7

int main(void)
{
    printf("STR(hello)        = %s\n", STR(hello));
    printf("STR(2 + 3)        = %s\n", STR(2 + 3));

    /* The classic surprise. */
    printf("STR(VERSION)      = %s   <-- the NAME, not the value\n", STR(VERSION));
    printf("XSTR(VERSION)     = %s   <-- the value\n", XSTR(VERSION));
    printf("XSTR(__LINE__)    = %s   <-- this line number\n", XSTR(__LINE__));

    /* ## builds identifiers. Handy for generated names, terrible for
     * readability -- grep will never find `my_counter` in this file. */
    int CAT(my_, counter) = 42;
    printf("my_counter        = %d\n", my_counter);

    /* The everyday use of # : a check that prints the expression that failed.
     * This is, essentially, how assert() is implemented. */
#define CHECK(cond) \
    printf("%-16s -> %s\n", #cond, (cond) ? "true" : "false")

    int n = 5;
    CHECK(n > 0);
    CHECK(n % 2 == 0);

    return 0;
}
