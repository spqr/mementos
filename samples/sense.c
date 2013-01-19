#include <common.h>

#ifdef __MSP430__
# include <msp430.h>
#else
# include <stdio.h>
#endif // __MSP430__

#define NUMSAMPLES 64

unsigned int progressbar = 0;

#ifdef __MSP430__
unsigned samples[NUMSAMPLES];
#else
const unsigned samples[NUMSAMPLES] = {
    81, 577, 207, 752, 327, 687, 212, 382, 512, 335, 770, 522, 357, 503, 850,
    118, 193, 188, 631, 38, 287, 853, 251, 644, 813, 374, 503, 507, 904, 522,
    287, 730, 13, 283, 64, 473, 971, 995, 805, 149, 876, 720, 707, 295, 514,
    465, 874, 252, 442, 770, 98, 185, 312, 2, 195, 785, 888, 52, 658, 94, 350,
    809, 502, 183
};
#endif // __MSP430__

unsigned sampmin, sampmax, sampmean, sampstddev;

/* adapted from http://supp.iar.com/FilesPublic/SUPPORT/000419/AN-G-002.pdf */
unsigned fast_sqrt (unsigned radicand) {
    unsigned a, b;
    b = radicand;
    a = radicand = 0x3f;
    radicand = b / radicand;
    a = radicand = (radicand+a)>>1;
    radicand = b / radicand;
    a = radicand = (radicand+a)>>1;
    radicand = b / radicand;
    radicand = (radicand+a)>>1;
    return radicand;
}

#ifdef __MSP430__
void setup (void) {
    P3DIR |= 0x01; // pin 3.0 is an output
}

void sense (void) {
    unsigned i;

  for (i = 0; i < NUMSAMPLES; ++i) {
    P3OUT |= 0x01; // pin 3.0 up

    /* do nothing for 100 cycles (~100 us) while accelerometer settles */
    asm volatile ("NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP");

    /* ADC code adapted from msp430f16x-samples/fet140_adc12_08.c */
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    ADC12CTL0 = SHT0_2 + ADC12ON;             // Set sampling time, turn on ADC12
    ADC12CTL1 = SHP;                          // Use sampling timer
    ADC12CTL0 |= ENC;                         // Conversion enabled
    P6SEL |= 0x01;                            // P6.0 ADC option select

    /* take three samples, one for each axis of the accelerometer */
#if 0
    for (ctr = 0; ctr < 3; ++ctr) {
        ADC12CTL0 |= ADC12SC;                   // Sampling open

        /* normally we'd busy-wait on the interrupt flag to wait for the end of
         * the ADC conversion, but that doesn't work in mspsim.  instead, wait 5
         * cycles (max ADC rate is ~200ksps, so 5us is reasonable */
        // while ((ADC12IFG & BIT0) == 0);         // wait for end of conversion
        asm volatile ("NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP");
    }
#endif

    /* manually unrolled version of above for loop */
    ADC12CTL0 |= ADC12SC;                   // Sampling open
    asm volatile ("NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP");
    ADC12CTL0 |= ADC12SC;                   // Sampling open
    asm volatile ("NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP");
    ADC12CTL0 |= ADC12SC;                   // Sampling open
    asm volatile ("NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP");

    samples[progressbar++] = ADC12MEM0; // grab the last of the 3 samples

    P3OUT &= ~0x01; // pin 3.0 down
  }
}
#endif // __MSP430__

unsigned divide (unsigned numerator, unsigned denominator) {
    return (numerator / denominator);
}

void compute (void) {
    unsigned i, sampval;
    unsigned min, max, mean, stddev, diff;
    long sumdiffs;

    min = samples[0];
    max = samples[0];
    mean = samples[0];

    for (i = 1; i < NUMSAMPLES; ++i) {
        sampval = samples[i];
        if (sampval < min) min = sampval;
        if (sampval > max) max = sampval;

        // running average
        // mean = (((i-1) * mean) + sampval) / i;
        mean = divide(((i-1) * mean) + sampval, i);
#ifndef __MSP430__
        printf("mean = %d\n", mean);
#endif // __MSP430__
    }

    sampmean = mean;
    sampmax = max;
    sampmin = min;

    sumdiffs = 0;
    for (i = 0; i < NUMSAMPLES; ++i) {
        diff = (samples[i] - mean);
        sumdiffs += diff * diff;
    }
    sampstddev = fast_sqrt(sumdiffs / NUMSAMPLES);
#ifndef __MSP430__
    printf("sampstddev = %d\n", sampstddev);
#endif // __MSP430__
}

#define SEG15 0xFE00
void store (void) {
    /* put stuff in flash segment 15 */
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + WRT;
    MEMREF(SEG15) = sampmin;
    MEMREF(SEG15+2) = sampmax;
    MEMREF(SEG15+4) = sampmean;
    MEMREF(SEG15+6) = sampstddev;
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
#ifdef __MSP430__
    WDTCTL = WDTPW + WDTHOLD; // stop WDT
    setup();
    sense();
#endif // __MSP430__
    compute();
#ifndef __MSP430__
    printf("sampmean = %d\n", sampmean);
    printf("sampmin = %d\n", sampmin);
    printf("sampmax = %d\n", sampmax);
#endif // __MSP430__
    store();
    return 91;
}
