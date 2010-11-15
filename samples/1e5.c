#include "common.h"

void foo (void) {
    register unsigned int i;
    for (i = 0; i < 65500; ++i) {
        asm("NOP");
    }
}

int main (void) {
    foo();
    return 0;
}
