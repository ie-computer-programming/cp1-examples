/* static_assert is checked at compile time and costs nothing at run time.
   To see one fail, build with -DSHOW_FAILURE and read the compiler error. */
#include <assert.h>
#include <stdio.h>

struct packet_header {
    unsigned char  version;
    unsigned char  flags;
    unsigned short length;
};

static_assert(sizeof(struct packet_header) == 4,
              "packet_header must be exactly 4 bytes on the wire");

#ifdef SHOW_FAILURE
static_assert(sizeof(int) == 2, "deliberately wrong: int is not 2 bytes here");
#endif

int main(void)
{
    printf("packet_header is %zu bytes\n", sizeof(struct packet_header));
    printf("int is %zu bytes\n", sizeof(int));
    return 0;
}
