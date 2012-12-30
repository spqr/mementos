#include <msp430x21x2.h>
#include "test.h"

#define INCH_VSENSE_IN INCH_6
#define VSENSE_POWER BIT3

unsigned int __mementos_voltage_check (void) {
    unsigned char i = 0;

    /* set up the ADC (a la blinky.c) */
    ADC12CTL0 &= ~ENC;
    ADC12CTL1 = INCH_VSENSE_IN + ADC12DIV_3;
    ADC12CTL0 = ADC12SHT_3 + ADC12ON + SREF_1 + REFON;// + REF2_5V;
    P3DIR = VSENSE_POWER;
    P3OUT |= VSENSE_POWER; // turn on the WISP's voltage sensing circuit

    for (i = 0; i < 50; ++i);

    ADC12CTL0 |= ENC + ADC12SC; // start the ADC sampling
    while (ADC12CTL1 & ADC12BUSY); // wait while it takes a sample
    ADC12CTL0 = 0; // turn off ADC
    P3OUT &= ~VSENSE_POWER; // turn off voltage sensing circuit

    /* ADC12MEM now contains 1024 * ((Vout/3) / 2.5V);
     * Vout = 3 * 2.5V * ADC12MEM / 1024;
     * scale up the 10-bit result to a full unsigned int */
    return ADC12MEM << 6;
}

int main (void) {
    register unsigned i;
    P1DIR |= BIT4;

    P1OUT |= BIT4;
    for (i = 0; i < 50; ++i);
    P1OUT &= ~BIT4;

    /* time between two GPIO spikes is the time for __mementos_voltage_check */
    (void)__mementos_voltage_check();
    /* spike width is the amount of time in __mementos_voltage_check due to
     * counting to 50 */

    P1OUT |= BIT4;
    for (i = 0; i < 50; ++i);
    P1OUT &= ~BIT4;
}
