/* 06-good -- typedef the struct, never the pointer. Keep the `*` visible.
 *
 * With the star written out, const goes where you can see it, and the
 * compiler enforces it.
 */
#include <stdio.h>

typedef struct point { int x, y; } Point;

static void show(const Point *p)       /* pointer to a const point */
{
    /* p->x = 999;   error: assignment of member 'x' in read-only object */
    printf("show: (%d, %d)\n", p->x, p->y);
}

int main(void)
{
    Point pt = {1, 2};
    show(&pt);
    printf("main: (%d, %d)\n", pt.x, pt.y);
    return 0;
}
