/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h>

extern unsigned int i, j, k;
extern unsigned int baseaddr;

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
     
    asm volatile ("PUSH 4(R1)");     // PC will appear at 28(R1)
    asm volatile ("PUSH R1");        // SP will appear at 26(R1)
    asm volatile ("ADD #6, 0(R1)"); // +6 /*brandon*/ to account for retaddr + PC + R1
    asm volatile ("PUSH R2");        // R2  will appear at 24(R1)
    // skip R3 (constant generator)
    asm volatile ("PUSH R4");        // R4  will appear at 22(R1)
    asm volatile ("PUSH R5");        // R5  will appear at 20(R1)
    asm volatile ("PUSH R6");        // R6  will appear at 18(R1)
    asm volatile ("PUSH R7");        // R7  will appear at 16(R1)
    asm volatile ("PUSH R8");        // R8  will appear at 14(R1)
    asm volatile ("PUSH R9");        // R9  will appear at 12(R1)
    asm volatile ("PUSH R10");       // R10 will appear at 10(R1)
    asm volatile ("PUSH R11");       // R11 will appear at 8(R1)
    asm volatile ("PUSH R12");       // R12 will appear at 6(R1)
    asm volatile ("PUSH R13");       // R13 will appear at 4(R1)
    asm volatile ("PUSH R14");       // R14 will appear at 2(R1)
    asm volatile ("PUSH R15");         // R15 will appear at 0(R1)

    /**** figure out where to put this checkpoint bundle ****/
    /* precompute the size of the stack portion of the bundle */
    asm volatile ("MOV 26(R1), %0" :"=m"(j)); // j = SP
    /* j now contains the pre-call value of the stack pointer */

    baseaddr = __mementos_locate_next_bundle();

    /********** phase #0: save size header (2 bytes) **********/
 
    //Save these registers so we can put them back after
    //we're done using them to compute the size of the stack
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
    asm volatile ("POP 32(R14)"); // R15
    asm volatile ("POP 30(R14)"); // R14
    asm volatile ("POP 28(R14)"); // R13
    asm volatile ("POP 26(R14)"); // R12
    asm volatile ("POP 24(R14)"); // R11
    asm volatile ("POP 22(R14)"); // R10
    asm volatile ("POP 20(R14)"); // R9
    asm volatile ("POP 18(R14)"); // R8
    asm volatile ("POP 16(R14)"); // R7
    asm volatile ("POP 14(R14)"); // R6
    asm volatile ("POP 12(R14)"); // R5
    asm volatile ("POP 10(R14)");  // R4
    // skip R3 (constant generator)
    asm volatile ("POP 8(R14)");  // R2
    asm volatile ("POP 6(R14)");  // R1
    asm volatile ("POP 4(R14)");    // R0

    /********** phase #2: checkpoint memory. **********/

    /* checkpoint the stack by walking from SP to ToS */
    /*There needs to be a "+ 2" on this expression to get 
      past the last register, which is at baseaddr + BSH + BSR.
      The constant 2 refers to the size of the last register*/
    k = baseaddr + BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS + 2;
    for (i = j; i < TOPOFSTACK; i += 2 /*sizeof(unsigned)*/) {
        MEMREF_UINT(k + (i - j)) = MEMREF_UINT(i);
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
    MEMREF_UINT(ACTIVE_BUNDLE_PTR) = baseaddr;
    __mementos_log_event(MEMENTOS_STATUS_COMPLETED_CHECKPOINT);

#ifdef MEMENTOS_TIMER
    // CCR0 = TIMER_INTERVAL; // XXX choose a good interval -- what are the units?
    ok_to_checkpoint = 0; // not OK to checkpoint until next timer fire
#endif
}

/* find a place in the reserved FRAM area for storage of a new bundle of state.
 * */
unsigned int __mementos_locate_next_bundle () {
    unsigned int baseaddr;
    unsigned int target;

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

unsigned int __mementos_find_active_bundle (void) {
    unsigned int active_ptr = MEMREF_UINT(ACTIVE_BUNDLE_PTR);
    if (__mementos_bundle_in_range(active_ptr))
        return active_ptr;
    return 0xffff;
}

void __mementos_atboot_cleanup (void) {
}

void __mementos_inactive_cleanup (unsigned int active_bundle_addr) {
}

unsigned int __mementos_bundle_in_range (unsigned int bun_addr) {
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
