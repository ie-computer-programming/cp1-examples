/* 03 -- the X-macro: write the list ONCE, generate the enum AND the names.
 *
 * 01 kept two lists in step by hand: the enum and the name table. Add a
 * colour to one and forget the other, and color_names[new] is NULL.
 * Session 11's preprocessor can generate both from a single list.
 */
#include <stdio.h>

#define COLOR_LIST(X) \
    X(RED)            \
    X(GREEN)          \
    X(BLUE)           \
    X(MAGENTA)

#define AS_ENUM(name)   COLOR_##name,
#define AS_STRING(name) #name,

typedef enum { COLOR_LIST(AS_ENUM) COLOR_COUNT } Color;

static const char *const color_names[] = { COLOR_LIST(AS_STRING) };

/* If the two ever disagree, the build fails rather than the program. */
_Static_assert(sizeof color_names / sizeof color_names[0] == COLOR_COUNT,
               "color_names out of step with Color");

int main(void)
{
    for (Color c = 0; c < COLOR_COUNT; c++) {
        printf("%d -> %s\n", c, color_names[c]);
    }
    /* cc -E 03-x-macro.c | tail : see the enum and the array it produced */
    return 0;
}
