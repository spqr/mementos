#include "common.h"

/*
 * A semi-intelligent implementation of modular exponentation. Ripped off from
 * page 244 of Bruce Schneier's Applied Cryptography, 2e, ISBN 0-471-11709-9.
 * Uses repeated squaring and applies modulus each iteration to keep result size
 * down. No BigInt types yet.  Not as good as left-to-right method which
 * allegedly allows use of a sliding window optimization.
 *
 * Originally from mementos/src/msp430-asm-classify r219.
 */

#ifndef __MSP430__
#include <msp430.h>
#include <stdio.h>
#endif // __MSP430__

unsigned short iters;
#define NUMBASES 8
const unsigned int base[NUMBASES] = {
    12345, 11, 38, 8809,
    39751, 10100, 30175, 22018
};
const unsigned int exponent = 37;
const unsigned int modulus = 41213; // 41213 is prime

unsigned int modpow(unsigned int b, unsigned int e, unsigned int m) {
    unsigned long result = 1;
    unsigned long tmp;

    while (e > 0) {
        if ((e & 1) == 1) {
            /* multiply in this bit's contribution while using modulus to keep
             * result small */
            tmp = result * b;
            result = tmp - (tmp/m) * m;
        }
        tmp = b * b;
        b = b - (tmp/m) * m;
        e >>= 1;
        ++iters;
    }

    return (result & 0xFFFFu);
}

MEMENTOS_MAIN_ATTRIBUTES
int main( void )
{
#ifdef __MSP430__
    WDTCTL = WDTPW + WDTHOLD; // stop WDT
#endif // __MSP430__

    unsigned long result;
    unsigned int tmpresult;
    unsigned char i;

    for (i = 0; i < NUMBASES; ++i) {
        result += tmpresult = modpow(base[i], exponent, modulus);
#ifndef __MSP430__
        printf("%d ^ %d (mod %d) = %d\n", base[i],
                exponent, modulus, tmpresult);
#endif // __MSP430__
    }
    return 39;
}
