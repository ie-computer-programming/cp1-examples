/* 05 -- conditional compilation: code the compiler never even sees.
 *
 * #if / #ifdef decide what gets passed on to the compiler. Code inside a
 * false branch is not "skipped at runtime" -- it is DELETED before
 * compilation, so it does not have to compile at all. That is how one source
 * file supports three operating systems.
 *
 * Try it:
 *     gcc -E 05-conditional-compilation.c | tail -30
 *     cc -DVERBOSE -o verbose 05-conditional-compilation.c
 *     cc -DNDEBUG  -o release 05-conditional-compilation.c
 */
#include <stdio.h>

/* 1. Platform selection. The compiler predefines these for you. */
#if defined(__APPLE__)
#  define PLATFORM "macOS"
#elif defined(__linux__)
#  define PLATFORM "Linux"
#elif defined(_WIN32)
#  define PLATFORM "Windows"
#else
#  define PLATFORM "something else"
#endif

/* 2. A feature switch the build turns on: cc -DVERBOSE ... */
#ifdef VERBOSE
#  define BANNER "verbose build"
#else
#  define BANNER "quiet build"
#endif

/* 3. #error stops the build with a message of your own. Use it to make an
 *    unsupported configuration fail loudly instead of mysteriously. */
#if __STDC_VERSION__ < 201112L
#  error "this course needs C11 or newer"
#endif

int main(void)
{
    printf("platform      : %s\n", PLATFORM);
    printf("build         : %s\n", BANNER);
    printf("__STDC_VERSION__ = %ld\n", __STDC_VERSION__);

    /* NDEBUG is the one every C programmer must know: it is what CMake's
     * Release build defines, and it is what deletes every assert(). */
#ifdef NDEBUG
    printf("NDEBUG is defined -- assert() is gone (Session 3)\n");
#else
    printf("NDEBUG is not defined -- assert() is live (Session 3)\n");
#endif

    return 0;
}
