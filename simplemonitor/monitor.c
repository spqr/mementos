/*
 * Simple monitoring program that receives 4-bit words from another device.  It
 * does so using 4+1 GPIO pins set up as follows:
 *
 *   P1.0 -- clock pin.  If DO_INTERRUPTS is defined, then P1.0 is set up such
 *           that a rising edge triggers an interrupt that causes P2.0-3 to be
 *           read.  Otherwise (DO_INTERRUPTS undefined) P1.0 is polled and only
 *           when the polling finds it up are P2.0-3 read.
 *   P2.0 -- input pin (lsb)
 *   P2.1 -- input pin
 *   P2.2 -- input pin
 *   P2.3 -- input pin (msb)
 *
 * The program talking to this monitoring program should act as follows:
 *   1. Raise some subset of four pins that are hardwired to our P2.0-2.3
 *   2. Raise an additional pin that is hardwired to our P1.0
 *   3. NOP for a few cycles
 *   4. Put all five pins back down
 */

// #define DO_INTERRUPTS

#include <msp430x21x1.h>
#include <string.h>
#ifdef __MSP430__ // mspgcc or clang
# include <signal.h>
#endif

#define START_ADDR 0x0280u // we'll write logged values to memory starting here
#define RESBYTES 64 // limits number of values we can write

static unsigned char *memptr = (unsigned char *)START_ADDR;
// static unsigned char lastsym = 0;
static unsigned numreads = 0;

#ifdef __MSP430__
int
#else
void
#endif
main (void) {
#ifdef DO_INTERRUPTS
    asm ("DINT"); // don't want to get interrupted during startup
#endif // DO_INTERRUPTS

    WDTCTL = WDTPW + WDTHOLD; // turn off watchdog timer

#ifdef DO_INTERRUPTS
    P1IE |= 0x01; // transition on P1.0 will trigger Port1_ISR
    P1IES &= ~0x01; // low-to-high transition on P1.0 triggers Port1_ISR
    P1IFG &= ~0x01; // clear P1.0 interrupt flag (none has been triggered yet)
#endif // DO_INTERRUPTS

    P2DIR = 0; // all P2 pins are inputs

    memset((unsigned char *)START_ADDR, 0, RESBYTES);

//    FCTL3 = FWKEY; FCTL1 = FWKEY + WRT;
//    while (lastsym != 0xE) { // MEMENTOS_STATUS_PROGRAM_COMPLETE (newmem.h)
    while (numreads < RESBYTES) {
#ifdef DO_INTERRUPTS
        __bis_SR_register(LPM4_bits + GIE); // enter LPM4 with interrupts enabled
#else
        while (!(P1IN & 0x1)); // wait for P1.0 to go up
#endif // DO_INTERRUPTS

#ifdef DO_INTERRUPTS
        // if we get here, an interrupt happened and we can collect a value
        asm ("DINT"); // don't want to get interrupted while collecting
#endif // DO_INTERRUPTS

        *(memptr++) = /*lastsym =*/ P2IN & 0x0F; // grab P2.0-2.3

#ifndef DO_INTERRUPTS
        while (P1IN & 0x1); // wait for P1.0 to go back down
#endif // DO_INTERRUPTS
        ++numreads;
    }
//    FCTL1 = FWKEY; FCTL3 = FWKEY + LOCK;
    return;
}

#ifdef DO_INTERRUPTS
#if !defined(__MSP430__) && !defined(__clang__)
#pragma vector=PORT1_VECTOR
__interrupt void
#elif defined(__clang__)
void Port1_ISR (void) __attribute__((interrupt(PORT1_VECTOR)));
void
#else
interrupt (PORT1_VECTOR)
#endif
Port1_ISR (void)
{
    __bic_SR_register_on_exit(LPM4_bits); // exit LPM4
    P1IFG &= ~0x01; // clear P1.0 interrupt flag
}
#endif // DO_INTERRUPTS
