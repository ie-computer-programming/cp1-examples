/* 09 -- array of structs vs struct of arrays: a first look at
 * data-oriented programming.
 *
 * Both hold the same numbers. The difference is what ends up in a cache line
 * when you only need ONE field.
 *
 *   AoS: [x y z hp][x y z hp][x y z hp] ...   summing x pulls in y, z, hp too
 *   SoA: [x x x x ...][y y y ...][hp hp ...]  summing x pulls in only x
 *
 * A cache line is 64 bytes. With AoS, 16 of every 64 bytes fetched are used.
 * With SoA, all 64. Same algorithm, same instruction count -- different
 * number of trips to main memory.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4000000

typedef struct {
    float x, y, z;
    int   hp;
} Entity;                 /* 16 bytes */

typedef struct {
    float *x, *y, *z;
    int   *hp;
} Entities;               /* four separate arrays */

static double seconds_since(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

int main(void)
{
    Entity *aos = malloc(N * sizeof *aos);
    Entities soa = {
        .x  = malloc(N * sizeof *soa.x),
        .y  = malloc(N * sizeof *soa.y),
        .z  = malloc(N * sizeof *soa.z),
        .hp = malloc(N * sizeof *soa.hp),
    };

    if (aos == NULL || soa.x == NULL || soa.y == NULL ||
        soa.z == NULL || soa.hp == NULL) {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < N; i++) {
        float v = (float)(i % 100);
        aos[i] = (Entity){.x = v, .y = v, .z = v, .hp = 100};
        soa.x[i] = v; soa.y[i] = v; soa.z[i] = v; soa.hp[i] = 100;
    }

    printf("sizeof(Entity) = %zu, N = %d, total = %.1f MB\n\n",
           sizeof(Entity), N, (double)(N * sizeof(Entity)) / 1e6);

    clock_t t = clock();
    double sum_aos = 0.0;
    for (size_t i = 0; i < N; i++) {
        sum_aos += aos[i].x;
    }
    double time_aos = seconds_since(t);

    t = clock();
    double sum_soa = 0.0;
    for (size_t i = 0; i < N; i++) {
        sum_soa += soa.x[i];
    }
    double time_soa = seconds_since(t);

    printf("sum of x, array of structs : %.0f in %.4f s\n", sum_aos, time_aos);
    printf("sum of x, struct of arrays : %.0f in %.4f s\n", sum_soa, time_soa);
    printf("\nsame answer, %.1fx\n", time_aos / time_soa);

    free(aos);
    free(soa.x); free(soa.y); free(soa.z); free(soa.hp);
    return 0;
}
