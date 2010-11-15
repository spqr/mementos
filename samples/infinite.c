#include "common.h"
#include <msp430x21x1.h>
#include "newmem.h"

unsigned int i = 0;

void infinite (void) {
    for (i = 0; i < 0xFFFEu; ++i) {
        asm volatile ("NOP");
    }
}

int main (void) {
    unsigned int i;
    infinite();
    return 0;
}
