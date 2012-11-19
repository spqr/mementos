#include "common.h"

void blargh () {
    unsigned a = 125;
    do {
        ++a;
    } while (a < 127);
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    blargh();
    return 0;
}
