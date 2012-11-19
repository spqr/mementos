#include "common.h"
#include "newmem.h"

#define NUMWRITES 256
#define BASEADDR 0xfa00

void writes_mnotp (void) {
    unsigned int i;
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + WRT;
    for (i = 0; i < 256; ++i) {
        MEMREF(BASEADDR + 2*i) = 0xf0f0u;
    }
    //FCTL1 = FWKEY;
    //FCTL3 = FWKEY + LOCK;
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    writes_mnotp();
    return 123;
}
