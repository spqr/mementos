/* test program for mementos event logging.  compile like:
   msp430-gcc -mmcu=msp430x2132 -o logme logme.c
   [ransford Mon Sep 27 17:28:52 2010]
 */

#include "common.h"
#include <msp430x21x2.h>

/* log a 4-bit event (lowest 4 bits of evt) to an external listening device.
 * see http://wisp.wikispaces.com/HW+Feature+Set to find free pins that aren't
 * hardwired to power-hungry peripherals.
 */
void __mementos_log_event (unsigned char evt) {
    P2OUT |= (evt & BIT0) << 3; // put bit 0/LSB of evt -> P2.3
    P3OUT |= (evt & BIT1) >> 1; // put bit 1 of evt     -> P3.0
    P3OUT |= (evt & BIT2) << 2; // put bit 2 of evt     -> P3.4
    P3OUT |= (evt & BIT3) << 2; // put bit 3 of evt     -> P3.5
    P1OUT |= BIT4; // raise the clock pin                  P1.4

    asm volatile ("NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP"); // wait a bit (xxx how much is enough?)

    // put the pins back down
    P1OUT &= ~BIT4;
    P2OUT &= ~BIT3;
    P3OUT &= ~(BIT0 | BIT4 | BIT5);
}

int main (void) {
    /* set up pins for event logging.  why can't we just use a contiguous range
     * like P3.0-4?  because some of those pins are hardwired to peripherals on
     * the WISP.  so we scavenge 4+1 pins from various ports according to
     * http://wisp.wikispaces.com/HW+Feature+Set */
    P1DIR |= BIT4;
    P2DIR |= BIT3;
    P3DIR |= BIT0 | BIT4 | BIT5;

    __mementos_log_event(0xA);
    __mementos_log_event(0xB);
    __mementos_log_event(0xC);
    __mementos_log_event(0xD);
}
