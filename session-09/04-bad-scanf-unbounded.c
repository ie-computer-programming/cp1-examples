/* 04-bad -- fscanf("%s") has no size, so it cannot fail safely.
 *
 * This is Session 7's strcpy problem wearing a different hat: the conversion
 * writes as many characters as the input happens to contain, into a buffer
 * whose size it was never told. A 34-character word into 8 bytes.
 */
#include <stdio.h>

int main(int argc, char **argv)
{
    const char *path = (argc > 1) ? argv[1] : "long-word.txt";

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror(path);
        return 1;
    }

    char word[8];
    fscanf(f, "%s", word);     /* no bound -- the input decides how much to write */

    printf("word = %s\n", word);

    fclose(f);
    return 0;
}
