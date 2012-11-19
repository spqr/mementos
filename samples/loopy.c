#include "common.h"
#include <msp430x21x1.h>
#include "newmem.h"

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    volatile unsigned int i;
    while (1) {
        i = VOLTAGE_CHECK;
    }
}
