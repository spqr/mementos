/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h>

#if defined(MEMENTOS_LOGGING) && defined(MEMENTOS_HARDWARE)
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
#else
#define __mementos_log_event(x) // no-op
#endif // MEMENTOS_LOGGING && MEMENTOS_HARDWARE
