/* 04-good -- give the conversion a width, or use fgets.
 *
 * Two safe options:
 *   fscanf(f, "%7s", word)   width is one LESS than the buffer ('\0' needs room)
 *   fgets(word, sizeof word, f)   takes the size directly -- prefer this
 *
 * The "%7s" form has the same defect as every hard-coded size: change the
 * buffer and the format string silently stops matching. fgets takes
 * `sizeof word`, so it cannot drift.
 */
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    const char *path = (argc > 1) ? argv[1] : "long-word.txt";

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror(path);
        return 1;
    }

    char word[8];

    /* fscanf returns the number of conversions that succeeded. */
    if (fscanf(f, "%7s", word) != 1) {
        fprintf(stderr, "no word in %s\n", path);
        fclose(f);
        return 1;
    }
    printf("fscanf(\"%%7s\") -> %s   (truncated, but safe)\n", word);

    rewind(f);

    char line[8];
    if (fgets(line, sizeof line, f) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        printf("fgets(sizeof)  -> %s   (truncated, but safe)\n", line);
    }

    fclose(f);
    return 0;
}
