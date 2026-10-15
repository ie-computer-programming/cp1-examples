/* 02-bad -- a C enum is an int with a name tag, and the tag is not checked.
 *
 * Each line below would be an error in Python (or in C++). In C, every one
 * of them compiles -- and two of them silently produce nonsense.
 */
#include <stdio.h>

typedef enum { COLOR_RED, COLOR_GREEN, COLOR_BLUE } Color;
typedef enum { FRUIT_APPLE, FRUIT_BANANA, FRUIT_CHERRY } Fruit;

static const char *color_name(Color c)
{
    switch (c) {
    case COLOR_RED:   return "red";
    case COLOR_GREEN: return "green";
    case COLOR_BLUE:  return "blue";
    }
    return "???";                  /* reached for any value outside 0..2 */
}

int main(void)
{
    Color a = FRUIT_BANANA;        /* a Fruit where a Color belongs */
    Color b = 42;                  /* no such colour */
    int   n = COLOR_BLUE * 10;     /* arithmetic on a "colour" */

    printf("a = %s\n", color_name(a));
    printf("b = %s\n", color_name(b));
    printf("n = %d\n", n);
    printf("FRUIT_CHERRY == COLOR_BLUE ? %s\n",
           (FRUIT_CHERRY == (Fruit)COLOR_BLUE) ? "yes" : "no");
    return 0;
}
