/* BAD: strcpy cannot fail, so it corrupts instead.
 *
 * strcpy takes no size. It copies until it finds a '\0' in the SOURCE and
 * writes wherever the destination happens to be, whether or not there is
 * room. The buffer below holds 8 bytes; the name is longer.
 *
 * This is the mechanism behind decades of remote code execution bugs. gets()
 * was so unfixable that C11 removed it from the language.
 *
 * Watch it get caught:
 *     ./build/07-bad-strcpy-asan
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char small[8];
    char after[8] = "SAFE";

    const char *name = "a name far too long for eight bytes";

    strcpy(small, name);      /* <-- writes ~36 bytes into 8 */

    printf("small = %s\n", small);
    printf("after = %s  <-- was \"SAFE\" before the copy\n", after);
    return 0;
}
