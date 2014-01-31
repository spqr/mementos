/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h>

#ifndef MEMENTOS_FRAM
#error Inappropriate use of mementos_fram.c without MEMENTOS_FRAM defined
#endif

void __mementos_checkpoint (void) {
    /* Size of globals in bytes.  Count this far from the beginning of RAM to
     * capture globals.  Comes from the AddGlobalSizeGlobal pass. */
    extern int GlobalAllocSize;
}

/* find a place in the reserved FRAM area for storage of a new bundle of state.
 * */
unsigned int __mementos_locate_next_bundle (unsigned int sp /* hack */) {
}

void __mementos_atboot_cleanup (void) {
}

void __mementos_inactive_cleanup (unsigned int active_bundle_addr) {
}

unsigned int __mementos_bundle_in_range (unsigned int bun_addr) {
    return 0; // XXX
}

unsigned int __mementos_force_free (void) {
}
