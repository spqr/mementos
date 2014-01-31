/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h>

#ifndef MEMENTOS_HARDWARE
#error Inappropriate use of mementos_hw.c without MEMENTOS_HARDWARE defined
#endif

/* Take a single ADC reading to determine capacitor voltage.
 * Code adapted from:
 *   - Intel WISP 4.1 firmware
 *   - 'blinky.c' from http://wisp.wikispaces.com/WISP+code+examples
 * Further advice from
 *   - http://wisp.wikispaces.com/ManagingPower
 * THIS CODE IS INTEL DLWISP 4.1 SPECIFIC! */
unsigned int __mementos_voltage_check (void) {
    unsigned char i = 0;

    __mementos_log_event(MEMENTOS_STATUS_CHECKING_VOLTAGE);

    /* set up the ADC (a la blinky.c) */
    ADC10CTL0 &= ~ENC;
    ADC10CTL1 = INCH_VSENSE_IN + ADC10DIV_3;
    ADC10CTL0 = ADC10SHT_3 + ADC10ON + SREF_1 + REFON;// + REF2_5V;
    P3DIR = VSENSE_POWER;
    P3OUT |= VSENSE_POWER; // turn on the WISP's voltage sensing circuit

    for (i = 0; i < 50; ++i);

    ADC10CTL0 |= ENC + ADC10SC; // start the ADC sampling
    while (ADC10CTL1 & ADC10BUSY); // wait while it takes a sample
    ADC10CTL0 = 0; // turn off ADC
    P3OUT &= ~VSENSE_POWER; // turn off voltage sensing circuit

    __mementos_log_event(MEMENTOS_STATUS_DONE_CHECKING_VOLTAGE);

    /* ADC10MEM now contains 1024 * ((Vout/3) / 2.5V);
     * Vout = 3 * 2.5V * ADC10MEM / 1024;
     * scale up the 10-bit result to a full unsigned int */
    return ADC10MEM << 6;
}
