#include <msp430.h>
#include "test.h"

void flasherase (void) {
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + ERASE;
    asm volatile("MOV.W #0, &0xFA00");
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

int main (void) {
    register unsigned i;
    P1DIR |= BIT4;

    P1OUT |= BIT4;
    for (i = 0; i < 50; ++i);
    P1OUT &= ~BIT4;

    /* time between two GPIO spikes is the time for flasherase */
    flasherase();

    P1OUT |= BIT4;
    for (i = 0; i < 50; ++i);
    P1OUT &= ~BIT4;

    return 3;
}
