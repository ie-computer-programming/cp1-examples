/* 06-bad -- a typedef that hides a `*` also hides where `const` applies.
 *
 *     typedef struct point *PointPtr;
 *     void show(const PointPtr p)
 *
 * reads as "a pointer to a const point". It is not. const applies to the
 * whole typedef'd type -- the POINTER -- so p cannot be reassigned, but the
 * point it aims at can be changed freely. It compiles without a warning.
 */
#include <stdio.h>

typedef struct point { int x, y; } Point;
typedef struct point *PointPtr;

static void show(const PointPtr p)     /* means:  struct point *const p */
{
    p->x = 999;                        /* "read-only" function writes anyway */
    printf("show: (%d, %d)\n", p->x, p->y);
}

int main(void)
{
    Point pt = {1, 2};
    show(&pt);
    printf("main: (%d, %d)   <-- changed by a function that promised not to\n",
           pt.x, pt.y);
    return 0;
}
