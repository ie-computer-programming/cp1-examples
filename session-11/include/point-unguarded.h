/* The SAME header, with the guard removed. Including it twice is a compile
 * error -- see the README. This file is deliberately not part of the build. */

struct point {
    int x;
    int y;
};

static inline int point_sum(struct point p)
{
    return p.x + p.y;
}
