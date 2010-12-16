#include <msp430x21x2.h>

void __stop_progExec__ (void) {
    _BIS_SR(CPUOFF);
}

void flashwrite (void) {
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + WRT;
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

    /* time between two GPIO spikes is the time for flashwrite */
    flashwrite();

    P1OUT |= BIT4;
    for (i = 0; i < 50; ++i);
    P1OUT &= ~BIT4;
}
