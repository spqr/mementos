#include "common.h"
#include "newmem.h"

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + WRT;
    ACTIVE_BUNDLE_PTR = 0x0123u;
    //FCTL1 = FWKEY;
    //FCTL3 = FWKEY + LOCK;
}
