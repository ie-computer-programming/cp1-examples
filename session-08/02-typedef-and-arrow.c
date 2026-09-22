/* 02 -- typedef, and the two ways to reach a member.
 *
 *   value.field     when you have the struct
 *   pointer->field  when you have a pointer to it
 *
 * `p->x` is nothing but shorthand for `(*p).x`. The parentheses are required
 * in the long form, because `.` binds tighter than `*` -- which is exactly
 * why the `->` shorthand exists.
 */
#include <stdio.h>

/* Without typedef the type is spelled `struct point` everywhere.
 * With it, `Point` alone is enough. Keep the tag (`point`) anyway: it is the
 * only way a struct can contain a pointer to its own type, because the
 * typedef name does not exist yet inside the braces. */
typedef struct point {
    int x;
    int y;
} Point;

int main(void)
{
    Point p = {.x = 3, .y = 4};
    Point *ptr = &p;

    printf("p.x         = %d\n", p.x);
    printf("(*ptr).x    = %d\n", (*ptr).x);
    printf("ptr->x      = %d\n", ptr->x);

    /* Writing through the pointer changes the original -- there is only one
     * Point here, and two names for it. */
    ptr->y = 100;
    printf("p.y after ptr->y = 100 : %d\n", p.y);

    printf("&p = %p, ptr = %p\n", (void *)&p, (void *)ptr);

    return 0;
}
