/* 01 -- enum basics, and the idioms that make enums useful in C.
 *
 * Python's enum.Enum is a class: members are objects, they know their own
 * names, and Color.RED == 0 is False. A C enum is a list of named int
 * constants -- nothing more. Everything Python gave you, you build by hand.
 */
#include <stdio.h>

typedef enum {
    COLOR_RED,          /* 0 -- starts at 0 */
    COLOR_GREEN,        /* 1 -- each one is the previous + 1 */
    COLOR_BLUE,         /* 2 */
    COLOR_COUNT         /* 3 -- not a colour: the NUMBER of colours */
} Color;

/* No reflection in C: the names exist only in your source. Designated
 * initialisers tie each string to its enumerator, so reordering is safe. */
static const char *const color_names[COLOR_COUNT] = {
    [COLOR_RED]   = "red",
    [COLOR_GREEN] = "green",
    [COLOR_BLUE]  = "blue",
};

typedef enum {
    HTTP_OK        = 200,     /* explicit values when they mean something */
    HTTP_NOT_FOUND = 404,
    HTTP_TEAPOT    = 418,
} HttpStatus;

int main(void)
{
    for (Color c = 0; c < COLOR_COUNT; c++) {
        printf("%d -> %s\n", c, color_names[c]);
    }

    printf("\nHTTP_NOT_FOUND = %d\n", HTTP_NOT_FOUND);
    printf("sizeof(Color) = %zu   (implementation-defined -- usually an int)\n",
           sizeof(Color));
    return 0;
}
