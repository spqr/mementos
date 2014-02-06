/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h>

extern unsigned int i, j, k;
extern unsigned long baseaddr;
unsigned long xxx = 0;

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
        return;
    }
#endif // MEMENTOS_TIMER
    /* early exit if voltage check says that checkpoint is unnecessary */
    if (VOLTAGE_CHECK >= V_THRESH) {
#ifdef MEMENTOS_TIMER
        ok_to_checkpoint = 0; // put the flag back down
#endif // MEMENTOS_TIMER
        asm volatile ("RET"); // would 'return', but that triggers a bug XXX
    }
#endif // MEMENTOS_ORACLE
    __mementos_log_event(MEMENTOS_STATUS_STARTING_CHECKPOINT);

    /* push all the registers onto the stack; they will be copied to NVRAM from
     * there.  any funny business here is to capture the values of the registers
     * as they appeared just before this function was called -- some
     * backtracking is necessary. */
    asm volatile ("PUSH 4(R1)\n\t"     // PC will appear at 56(R1)
                  "PUSH R1\n\t"        // SP will appear at 52(R1)
                  "ADD #12, 0(R1)\n\t" // +12 to account for retaddr + PC + R1
                  "PUSH R2\n\t"        // R2  will appear at 48(R1)
                                       // skip R3 (constant generator)
                  "PUSH R4\n\t"        // R4  will appear at 44(R1)
                  "PUSH R5\n\t"        // R5  will appear at 40(R1)
                  "PUSH R6\n\t"        // R6  will appear at 36(R1)
                  "PUSH R7\n\t"        // R7  will appear at 32(R1)
                  "PUSH R8\n\t"        // R8  will appear at 28(R1)
                  "PUSH R9\n\t"        // R9  will appear at 24(R1)
                  "PUSH R10\n\t"       // R10 will appear at 20(R1)
                  "PUSH R11\n\t"       // R11 will appear at 16(R1)
                  "PUSH R12\n\t"       // R12 will appear at 12(R1)
                  "PUSH R13\n\t"       // R13 will appear at 8(R1)
                  "PUSH R14\n\t"       // R14 will appear at 4(R1)
                  "PUSH R15");         // R15 will appear at 0(R1)

    /**** figure out where to put this checkpoint bundle ****/
    /* precompute the size of the stack portion of the bundle */
    asm volatile ("MOV 52(R1), %0" :"=m"(j)); // j = SP
    /* j now contains the pre-call value of the stack pointer */

    baseaddr = __mementos_locate_next_bundle();

    /********** phase #0: save size header (2 bytes) **********/
    asm volatile ("PUSH R12");
    asm volatile ("PUSH R13");

    // compute size of stack (in bytes) into R13
    asm volatile ("MOV #" xstr(TOPOFSTACK) ", R13");
    asm volatile ("SUB %0, R13" ::"m"(j)); // j == old stack pointer

    // write size of stack (R13) to high word at baseaddr
    asm volatile ("MOV %0, R12" ::"m"(baseaddr));
    asm volatile ("MOV R13, 2(R12)");

    // store GlobalAllocSize into R13, round it up to the next word boundary
    asm volatile ("MOV %0, R13" ::"m"(GlobalAllocSize));
    asm volatile ("INC R13");
    asm volatile ("AND #0xFE, R13");

    // write GlobalAllocSize to low word at baseaddr
    asm volatile ("MOV R13, 0(R12)");

    asm volatile ("POP R13");
    asm volatile ("POP R12");

    /********** phase #1: checkpoint registers. **********/
    asm volatile ("MOV %0, R14" ::"m"(baseaddr));
    asm volatile ("POP 60(R14)\n\t" // R15
                  "POP 56(R14)\n\t" // R14
                  "POP 52(R14)\n\t" // R13
                  "POP 48(R14)\n\t" // R12
                  "POP 44(R14)\n\t" // R11
                  "POP 40(R14)\n\t" // R10
                  "POP 36(R14)\n\t" // R9
                  "POP 32(R14)\n\t" // R8
                  "POP 28(R14)\n\t" // R7
                  "POP 24(R14)\n\t" // R6
                  "POP 20(R14)\n\t" // R5
                  "POP 16(R14)\n\t"  // R4
                  // skip R3 (constant generator)
                  "POP 12(R14)\n\t"  // R2
                  "POP 8(R14)\n\t"  // R1
                  "POP 4(R14)");    // R0
    asm volatile ("MOV R14, %0" :"=m"(xxx):);

    /********** phase #2: checkpoint memory. **********/

    /* checkpoint the stack by walking from SP to ToS */
    k = baseaddr + BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS;
    for (i = j; i < TOPOFSTACK; i += sizeof(unsigned long)) {
        MEMREF_ULONG(k + (i - j)) = MEMREF_ULONG(i);
    }
    k += (i - j); // skip over checkpointed stack

    /* checkpoint as much of the data segment as is necessary */
    for (i = STARTOFDATA; i < STARTOFDATA+ROUND_TO_NEXT_EVEN(GlobalAllocSize);
            i += sizeof(unsigned int)) {
        MEMREF_UINT(k + (i - STARTOFDATA)) = MEMREF_UINT(i);
    }
    k += (i - STARTOFDATA); // skip over checkpointed globals

    // write the magic number
    MEMREF_UINT(k) = MEMENTOS_MAGIC_NUMBER;
    MEMREF_ULONG(ACTIVE_BUNDLE_PTR) = baseaddr;
    __mementos_log_event(MEMENTOS_STATUS_COMPLETED_CHECKPOINT);

#ifdef MEMENTOS_TIMER
    // CCR0 = TIMER_INTERVAL; // XXX choose a good interval -- what are the units?
    ok_to_checkpoint = 0; // not OK to checkpoint until next timer fire
#endif
}

/* find a place in the reserved FRAM area for storage of a new bundle of state.
 * */
unsigned long __mementos_locate_next_bundle () {
    unsigned long baseaddr;
    unsigned long target;

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
