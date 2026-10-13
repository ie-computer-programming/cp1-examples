/* 03-good -- parenthesise every bitwise test. Always. */
#include <stdio.h>

#define READY 0x1u

int main(void)
{
    unsigned flags = 0x6;

    if ((flags & READY) == 0) {
        printf("not ready -- waiting\n");
    } else {
        printf("ready -- starting\n");
    }
    return 0;
}
