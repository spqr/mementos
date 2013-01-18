#include "common.h"
#include <msp430.h>

unsigned int iters = 0;

/**
 * CRC-16 from WISP firmware 4.1.  Comment from their code follows:
 * <quote>
 * This code comes from the Open Tag Systems Protocol Reference Guide version
 * 1.1 dated 3/23/2004.
 * (http://www.opentagsystems.com/pdfs/downloads/OTS_Protocol_v11.pdf) No
 * licensing information accompanied the code snippet.
 * </quote>
 **/
unsigned short crc16_ccitt_mnotp(volatile unsigned char *data, unsigned short n) {
    register unsigned short i, j;
    unsigned short crc_16;

    crc_16 = 0xFFFFu; // Equivalent Preset to 0x1D0F
    for (i=0; i<n; i++) {
        crc_16^=data[i] << 8;
        for (j = 0; j < 8; ++j) {
            if (crc_16 & 0x8000) {
                crc_16 <<= 1;
                crc_16 ^= 0x1021; // (CCITT) x16 + x12 + x5 + 1
            } else {
                crc_16 <<= 1;
            }
        }
    }
    return(crc_16^0xFFFFu);
}

void do_crc (void) {
    // check the upper halves of eight 512B flash segments
    volatile unsigned short accumulator = 0;
    for (iters = 0; iters < 64; ++iters)
        accumulator += crc16_ccitt_mnotp((unsigned char *)(0xE000 + iters*64), 32);
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    WDTCTL = WDTPW + WDTHOLD; // stop WDT

    do_crc();
    return 29; // to indicate success
}
