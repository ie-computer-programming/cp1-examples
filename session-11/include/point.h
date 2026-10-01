/* A header with an include guard -- include it as many times as you like. */
#ifndef CP1_POINT_H          /* if this name has not been defined yet ... */
#define CP1_POINT_H          /* ... define it, so the next #include skips all this */

struct point {
    int x;
    int y;
};

static inline int point_sum(struct point p)
{
    return p.x + p.y;
}

#endif /* CP1_POINT_H */
