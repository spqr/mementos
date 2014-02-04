/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h>

extern unsigned int i, j, k;
extern unsigned long baseaddr;

unsigned int GlobalAllocSize = 0;

#ifndef MEMENTOS_FRAM
#error Inappropriate use of mementos_fram.c without MEMENTOS_FRAM defined
#endif

void __mementos_checkpoint (void) {
    /* Size of globals in bytes.  Count this far from the beginning of RAM to
     * capture globals.  Comes from the AddGlobalSizeGlobal pass. */
    //extern int GlobalAllocSize;

#ifndef MEMENTOS_ORACLE // always checkpoint when called in oracle mode
#ifdef MEMENTOS_TIMER
    /* early exit if not ok to checkpoint */
    if (!ok_to_checkpoint) {
        /*
        // reenable interrupts -- but only if they were enabled when we started
        if (interrupts_enabled & 0x8) // GIE bit in SR/R2 (see MSP430 manual)
            asm volatile ("EINT");
        */
        return;
    }
#endif // MEMENTOS_TIMER
    /* early exit if voltage check says that checkpoint is unnecessary */
    if (VOLTAGE_CHECK >= V_THRESH) {
        /*
        // reenable interrupts -- but only if they were enabled when we started
        if (interrupts_enabled & 0x8) // GIE bit in SR/R2 (see MSP430 manual)
            asm volatile ("EINT");
        */
#ifdef MEMENTOS_TIMER
        ok_to_checkpoint = 0; // put the flag back down
#endif // MEMENTOS_TIMER
        asm volatile ("RET"); // would 'return', but that triggers a bug XXX
    }
#endif // MEMENTOS_ORACLE
    __mementos_log_event(MEMENTOS_STATUS_STARTING_CHECKPOINT);

    // push all the registers onto the stack
    asm volatile ("PUSH 2(R1)\n\t" // PC will appear at 28(R1)
                  "PUSH R1\n\t"    // SP will appear at 26(R1)
                  "ADD #6, 0(R1)\n\t" // to account for 2xPC + R1 itself
                  "PUSH R2\n\t"    // R2  will appear at 24(R1)
                //"PUSH R3\n\t" // skip R3
                  "PUSH R4\n\t"    // R4  will appear at 22(R1)
                  "PUSH R5\n\t"    // R5  will appear at 20(R1)
                  "PUSH R6\n\t"    // R6  will appear at 18(R1)
                  "PUSH R7\n\t"    // R7  will appear at 16(R1)
                  "PUSH R8\n\t"    // R8  will appear at 14(R1)
                  "PUSH R9\n\t"    // R9  will appear at 12(R1)
                  "PUSH R10\n\t"   // R10 will appear at 10(R1)
                  "PUSH R11\n\t"   // R11 will appear at 8(R1)
                  "PUSH R12\n\t"   // R12 will appear at 6(R1)
                  "PUSH R13\n\t"   // R13 will appear at 4(R1)
                  "PUSH R14\n\t"   // R14 will appear at 2(R1)
                  "PUSH R15");     // R15 will appear at 0(R1)

    /**** figure out where to put this checkpoint bundle ****/
    /* precompute the size of the stack portion of the bundle */
    asm volatile ("MOV 26(R1), %0" :"=m"(j)); // j = SP
    /* j now contains the pre-call value of the stack pointer */

    baseaddr = __mementos_locate_next_bundle(j);

    /********** phase #0: save size header (2 bytes) **********/
    asm volatile ("PUSH R12");
    asm volatile ("PUSH R13");

    // compute size of stack (in bytes) into R13
    asm volatile ("MOV #" xstr(TOPOFSTACK) ", R13");
    asm volatile ("SUB %0, R13" ::"m"(j)); // j == old stack pointer

    // write size of stack (R13) to high byte at baseaddr
    asm volatile ("MOV %0, R12" ::"m"(baseaddr));
    asm volatile ("MOV.B R13, 1(R12)");

    // store GlobalAllocSize into R13, round it up to the next word boundary
    asm volatile ("MOV %0, R13" ::"m"(GlobalAllocSize));
    asm volatile ("INC R13");
    asm volatile ("AND #0xFE, R13");

    // write GlobalAllocSize to low byte at baseaddr
    asm volatile ("MOV.B R13, 0(R12)");

    asm volatile ("POP R13");
    asm volatile ("POP R12");

    /********** phase #1: checkpoint registers. **********/
    asm volatile ("MOV %0, R14" ::"m"(baseaddr));
    asm volatile ("POP 30(R14)\n\t" // R15
                  "POP 28(R14)\n\t" // R14
                  "POP 26(R14)\n\t" // R13
                  "POP 24(R14)\n\t" // R12
                  "POP 22(R14)\n\t" // R11
                  "POP 20(R14)\n\t" // R10
                  "POP 18(R14)\n\t" // R9
                  "POP 16(R14)\n\t" // R8
                  "POP 14(R14)\n\t" // R7
                  "POP 12(R14)\n\t" // R6
                  "POP 10(R14)\n\t" // R5
                  "POP 8(R14)\n\t"  // R4
                  // skip R3 (constant generator)
                  "POP 6(R14)\n\t"  // R2
                  "POP 4(R14)\n\t"  // R1
                  "POP 2(R14)");    // R0

    /********** phase #2: checkpoint memory. **********/

    /* checkpoint the stack */
    i = j;
    j = TOPOFSTACK - i;
    while ((TOPOFSTACK - i) > 0) { // walk up from SP to ToS & copy to flash
        MEMREF_UINT((baseaddr + BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS + j-(TOPOFSTACK-i))) = MEMREF_UINT(i);
        i += 2;
    }

    /* checkpoint as much of the data segment as is necessary */
    j += BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS;
    for (i = STARTOFDATA; i < STARTOFDATA+ROUND_TO_NEXT_EVEN(GlobalAllocSize);
            i += 2, j += 2) {
        MEMREF_UINT(baseaddr + j) = MEMREF_UINT(i);
    }
    // write the magic number
    MEMREF_ULONG(baseaddr + j) = MEMENTOS_MAGIC_NUMBER;
    MEMREF_ULONG(ACTIVE_BUNDLE_PTR) = baseaddr;
    __mementos_log_event(MEMENTOS_STATUS_COMPLETED_CHECKPOINT);

#ifdef MEMENTOS_TIMER
    // CCR0 = TIMER_INTERVAL; // XXX choose a good interval -- what are the units?
    ok_to_checkpoint = 0; // not OK to checkpoint until next timer fire
#endif
}

/* find a place in the reserved FRAM area for storage of a new bundle of state.
 * */
unsigned long __mementos_locate_next_bundle (unsigned long sp /* hack */) {
    //extern int GlobalAllocSize;
    unsigned int size, b, bsize, next;
    unsigned long baseaddr;
    unsigned long target;

    /* how big is the bundle we'd like to store? */
    size =
        BUNDLE_SIZE_HEADER // bytes for the bundle header
        + BUNDLE_SIZE_REGISTERS // bytes for the register portion
        + (TOPOFSTACK - sp) // bytes for the stack portion
        + ROUND_TO_NEXT_EVEN(GlobalAllocSize) // bytes for the dataseg portion
        + BUNDLE_SIZE_MAGIC; // bytes for the magic number

    /* where does the currently active bundle start? */
    baseaddr = __mementos_find_active_bundle();
    switch (baseaddr) {
    case FRAM_FIRST_BUNDLE_SEG:
        target = FRAM_SECOND_BUNDLE_SEG;
        break;
    case FRAM_SECOND_BUNDLE_SEG:
        target = FRAM_FIRST_BUNDLE_SEG;
    default: // case 0xFFFFu:
        target = FRAM_FIRST_BUNDLE_SEG;
        break;
    }
    //__mementos_fram_clear(target);
    return target;
}

unsigned long __mementos_find_active_bundle (void) {
    unsigned long active_ptr = MEMREF_ULONG(ACTIVE_BUNDLE_PTR);
    if (__mementos_bundle_in_range(active_ptr))
        return active_ptr;
    return 0xffff;
}

void __mementos_atboot_cleanup (void) {
}

void __mementos_inactive_cleanup (unsigned int active_bundle_addr) {
}

unsigned int __mementos_bundle_in_range (unsigned long bun_addr) {
    return ((bun_addr >= FRAM_FIRST_BUNDLE_SEG) && (bun_addr < FRAM_END));
}

/*
void __mementos_fram_clear (unsigned long target) {
    unsigned long i;
    for (i = target; i < (target + (FRAM_SECOND_BUNDLE_SEG - FRAM_FIRST_BUNDLE_SEG)); ++i) {
        MEMREF(i) = 0;
    }
}
*/

unsigned int __mementos_force_free (void) {
    // XXX should never need this, but should implement anyway
}
