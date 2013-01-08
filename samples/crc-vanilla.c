#include "common.h"
#include "msp430.h"

/**
 * CRC-16 from WISP firmware 4.1.  Comment from their code follows:
 * <quote>
 * This code comes from the Open Tag Systems Protocol Reference Guide version
 * 1.1 dated 3/23/2004.
 * (http://www.opentagsystems.com/pdfs/downloads/OTS_Protocol_v11.pdf) No
 * licensing information accompanied the code snippet.
 * </quote>
 **/
unsigned short crc16_ccitt(volatile unsigned char *data, unsigned short n) {
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
    unsigned int i;
    for (i = 0; i < 8; ++i)
        accumulator += crc16_ccitt((unsigned char *)(0xE000 + i*512), 0x100);
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    WDTCTL = WDTPW + WDTHOLD; // stop WDT

    do_crc();
    return 27; // to indicate success
}
