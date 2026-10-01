/* 04-bad -- a macro argument is pasted once per appearance in the body.
 *
 *     #define MAX(a, b) ((a) > (b) ? (a) : (b))
 *
 * `a` appears twice. Pass something with a side effect and that side effect
 * happens TWICE:
 *
 *     MAX(i++, j++)  ->  ((i++) > (j++) ? (i++) : (j++))
 *
 * This is the same defect as `assert(fclose(f) == 0)` from Session 3: the
 * side effect is hidden inside something that does not behave like a call.
 */
#include <stdio.h>

#define MAX(a, b)  ((a) > (b) ? (a) : (b))

int main(void)
{
    int i = 5, j = 3;

    printf("before : i = %d, j = %d\n", i, j);

    int m = MAX(i++, j++);

    printf("MAX(i++, j++) = %d\n", m);
    printf("after  : i = %d, j = %d   <-- i incremented TWICE\n", i, j);

    printf("exit code will be 0\n");
    return 0;
}
