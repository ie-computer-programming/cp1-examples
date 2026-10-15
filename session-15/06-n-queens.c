/* 06 -- N-queens: backtracking with PRUNING.
 *
 * Place N queens on an N x N board so that no two attack each other.
 * One queen per row, so the question is only: which column in each row?
 *
 * Brute force tries all N^N column choices. Backtracking abandons a partial
 * board the moment two queens clash -- it never explores the millions of
 * boards that extend an already-impossible one.
 *
 * Columns and both diagonals are tracked in arrays, so "is this square
 * attacked?" is three lookups, not a scan of the board.
 */
#include <stdio.h>

#define MAXN 12

static int  n;
static int  col_of[MAXN];                 /* col_of[row] = column of that row's queen */
static int  col_used[MAXN];
static int  diag1[2 * MAXN];              /* row + col is constant on a / diagonal  */
static int  diag2[2 * MAXN];              /* row - col is constant on a \ diagonal  */
static long solutions;
static long placements_tried;

static void place(int row)
{
    if (row == n) {
        solutions++;
        return;
    }
    for (int c = 0; c < n; c++) {
        placements_tried++;
        if (col_used[c] || diag1[row + c] || diag2[row - c + n]) {
            continue;                     /* prune: this branch can never succeed */
        }
        col_of[row] = c;
        col_used[c] = diag1[row + c] = diag2[row - c + n] = 1;    /* choose    */
        place(row + 1);                                           /* explore   */
        col_used[c] = diag1[row + c] = diag2[row - c + n] = 0;    /* un-choose */
    }
}

static int first_found;

static void place_first(int row)
{
    if (first_found) {
        return;
    }
    if (row == n) {
        first_found = 1;
        return;
    }
    for (int c = 0; c < n && !first_found; c++) {
        if (col_used[c] || diag1[row + c] || diag2[row - c + n]) {
            continue;
        }
        col_of[row] = c;
        col_used[c] = diag1[row + c] = diag2[row - c + n] = 1;
        place_first(row + 1);
        if (!first_found) {
            col_used[c] = diag1[row + c] = diag2[row - c + n] = 0;
        }
    }
}

int main(void)
{
    n = 8;
    place_first(0);
    printf("one solution for 8 queens:\n");
    for (int r = 0; r < n; r++) {
        printf("  ");
        for (int c = 0; c < n; c++) {
            printf("%c ", col_of[r] == c ? 'Q' : '.');
        }
        printf("\n");
    }

    printf("\n  N   solutions   placements tried   brute force N^N\n");
    for (n = 4; n <= 10; n++) {
        for (int i = 0; i < 2 * MAXN; i++) {
            diag1[i] = diag2[i] = 0;
        }
        for (int i = 0; i < MAXN; i++) {
            col_used[i] = 0;
        }
        solutions = placements_tried = 0;
        place(0);

        unsigned long long brute = 1;
        for (int i = 0; i < n; i++) {
            brute *= (unsigned long long)n;
        }
        printf("%3d %11ld %18ld %17llu\n", n, solutions, placements_tried, brute);
    }
    return 0;
}
