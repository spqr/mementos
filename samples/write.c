#include "common.h"
#include <msp430x21x2.h>

int main (void) {
    P1DIR = 1;
    P1OUT = 1;
    return 9;
}
