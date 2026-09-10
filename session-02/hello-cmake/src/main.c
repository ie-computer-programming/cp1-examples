#include "greet.h"

int main(int argc, char *argv[])
{
    greet(argc > 1 ? argv[1] : "Computer Programming 1");
    return 0;
}
