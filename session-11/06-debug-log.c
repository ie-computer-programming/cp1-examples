/* 06 -- the debug-logging macro Session 3 promised you.
 *
 * Session 3 said: scatter fprintf(stderr, "%s:%d: ...", __func__, __LINE__)
 * while debugging, then delete them all before you commit. This is the
 * version you do not have to delete -- one -DNDEBUG and every call site
 * expands to nothing.
 *
 * It uses four things at once:
 *   __FILE__ __LINE__ __func__   where the message came from
 *   __VA_ARGS__                  "and the rest of the arguments"
 *   #ifdef NDEBUG                compile it out entirely
 *   a real function behind it    so the format string is still type-checked
 *
 * Try it:
 *     cc -o debug   06-debug-log.c
 *     cc -DNDEBUG -o release 06-debug-log.c      <-- silent
 */
#include <stdarg.h>
#include <stdio.h>

#ifdef NDEBUG

   /* ((void)0) rather than nothing: the call site is still a valid
    * expression statement, so `if (x) LOG("..."); else ...` keeps working. */
#  define LOG(...) ((void)0)

#else

static void cp1_log(const char *file, int line, const char *func,
                    const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "[%s:%d %s] ", file, line, func);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);

    va_end(args);
}

#  define LOG(...) cp1_log(__FILE__, __LINE__, __func__, __VA_ARGS__)

#endif

static int divide(int a, int b)
{
    LOG("dividing %d by %d", a, b);

    if (b == 0) {
        LOG("refusing to divide by zero");
        return 0;
    }
    return a / b;
}

int main(void)
{
    LOG("starting");

    printf("10 / 2 = %d\n", divide(10, 2));
    printf("10 / 0 = %d\n", divide(10, 0));

    LOG("done");

    printf("rebuild with -DNDEBUG and every LOG above vanishes\n");
    return 0;
}
