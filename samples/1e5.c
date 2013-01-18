#include "common.h"
#include <msp430.h>

unsigned int x = 0;

int foo (void) {
    register unsigned int i;
    for (i = 0; i < 1000; ++i) {
        asm volatile("NOP");
    }
    return 0xeeee;
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    WDTCTL = WDTPW + WDTHOLD; // stop WDT
    for (x = 0; x < 80; ++x) {
        (void)foo();
    }
    return 99;
}
