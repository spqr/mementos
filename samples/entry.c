#include "common.h"
#include "newmem.h"

volatile int foo;

void subentry (void) {
    asm volatile ("NOP");
    asm volatile ("NOP");
    (void)__mementos_checkpoint();
}

void entry (void) {
    asm volatile ("NOP");
    subentry();
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    __mementos_set_checkpoint_flag(0xFA00u);
    entry();
    asm volatile ("NOP");
}
