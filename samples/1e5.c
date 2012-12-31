#include "common.h"
#include <msp430.h>

unsigned int x;

void foo (void) {
    register unsigned int i;
    for (i = 0; i < 10000; ++i) {
        asm volatile("NOP");
    }
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    WDTCTL = WDTPW + WDTHOLD; // stop WDT
    for (x = 0; x < 10; ++x) {
        foo();
    }
    return 99;
}
