#include "common.h"

void foo (void) {
    register unsigned int i;
    for (i = 0; i < 65500; ++i) {
        asm("NOP");
    }
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    foo();
    return 0;
}
