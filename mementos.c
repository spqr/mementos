/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h> // XXX hack

// extern int _old_main (void);
unsigned long baseaddr;
unsigned long i, j, k;
unsigned long tmpsize;
// unsigned int interrupts_enabled;
#ifdef MEMENTOS_TIMER
bool ok_to_checkpoint;
#endif // MEMENTOS_TIMER

void __mementos_restore (unsigned long b) {
    /* b is a pointer to a valid bundle found by __mementos_find_active_bundle.
     * the first word of the bundle is split: the high byte designates the size
     * (in bytes) of the stack portion of the bundle and the low byte designates
     * the size (in bytes) of the data-segment portion of the bundle. */
    baseaddr = b; // XXX

    __mementos_log_event(MEMENTOS_STATUS_STARTING_RESTORATION);

    /*
    // disable interrupts -- they wouldn't be helpful here
    asm volatile ("DINT");
    */

    /* restore the stack by walking from the top to the bottom of the stack
     * portion of the checkpoint */
    tmpsize = MEMREF_UINT(baseaddr) >> 8; // stack size
    for (i = 0; i < tmpsize; i += 2) {
        /* summary:
        MEMREF(TOPOFSTACK - i) = MEMREF(baseaddr + 30 + tmpsize - i);
        */

        // j = TOPOFSTACK - i - 2;
        asm volatile("MOV #" xstr(TOPOFSTACK) ", %0" :"=m"(j));
        asm volatile("SUB %0, %1" :"=m"(i) :"m"(j));
        asm volatile("DECD.W %0" ::"m"(j));

        // k = baseaddr; k += 30; k += tmpsize; k -= i;
        asm volatile("MOV %1, %0" :"=m"(k) :"m"(baseaddr));
        asm volatile("ADD #" xstr(BUNDLE_SIZE_REGISTERS) ", %0" ::"m"(k));
        asm volatile("ADD %1, %0" :"=m"(k) :"m"(tmpsize));
        asm volatile("SUB %1, %0" :"=m"(k) :"m"(i));

        // MEMREF(j) = MEMREF(k);
        asm volatile("MOV %0, R7" ::"m"(k));
        asm volatile("MOV %0, R8" ::"m"(j));
        asm volatile("MOV @R7, 0(R8)");
    }

    /* restore the data segment without trampling on our own globals.
     * pseudocode:
     *
     * baseaddr = beginning of checkpoint bundle
     * stacksize = size of stack portion of checkpoint
     * regfilesize = size of register portion of checkpoint (30 bytes)
     * headersize = size of bundle header (2 bytes)
     *
     * for (i = 0; i < globalsize; i += 2) {
     *     memory[STARTOFDATA + i] = 
     *         memory[baseaddr + stacksize + regfilesize + headersize + i]
     * }
     */
    asm volatile("MOV %0, R7" ::"m"(tmpsize));  // R7(stacksize) = tmpsize
    asm volatile("MOV %0, R6" ::"m"(baseaddr)); // R6(baseaddr)  = baseaddr
    asm volatile("MOV @R6, R8\n\t"               // R8(globalsize) =
                 "AND #255, R8");                //   MEMREF(baseaddr) & 0x00FF
    asm volatile("CLR.W R9");                    // R9(i) = 0 // induction var

    asm volatile("rdloop:");                     // will jump back up here
    asm volatile("CMP R8, R9\n\t"                // if (i(R9) >= globalsize(R8))
                 "JC afterrd");                  //   <stop looping>
    asm volatile("MOV R6, R10\n\t"               // R10 = baseaddr(R6)
                 "ADD #" xstr(STACK_OFFSET) ", R10\n\t"              //   + 32 // skip regs, header,
                 "ADD R7, R10\n\t"               //   + stacksize(R7) // stack,
                 "ADD R9, R10");                 //   + i(R9)
    asm volatile("MOV @R10, "                    // MEMREF(STARTOFDATA+i(R9)) =
            xstr(STARTOFDATA) "(R9)");           //        MEMREF(R10)
    asm volatile("INCD R9");                     // i += 2
    asm volatile("JMP rdloop");                  // to beginning of loop

    asm volatile("afterrd:");                    // jump here when done

    /* set baseaddr back to whatever it was */
    asm volatile("MOV R6, %0" :"=m"(baseaddr));

    /* finally, restore all the registers, starting at R15 and counting down to
     * R0/PC.  setting R0/PC is an implicit jump, so we have to do it last. */

    /* j = <PC to restore> (note: R6 still contains baseaddr) */
    asm volatile ("MOV 2(R6), %0" :"=m"(j));

    /* set the SP first, so we can PUSH stuff; we'll reset it later */
    asm volatile ("MOV 4(R6), R1");
    /* now push the saved register values onto the stack (R6=baseaddr) */
    asm volatile ("PUSH 30(R6)\n\t"  // R15 is at 28(SP)
                  "PUSH 28(R6)\n\t"  // R14 is at 26(SP)
                  "PUSH 26(R6)\n\t"  // R13 is at 24(SP)
                  "PUSH 24(R6)\n\t"  // R12 is at 22(SP)
                  "PUSH 22(R6)\n\t"  // R11 is at 20(SP)
                  "PUSH 20(R6)\n\t"  // R10 is at 18(SP)
                  "PUSH 18(R6)\n\t"  // R9 is at 16(SP)
                  "PUSH 16(R6)\n\t"  // R8 is at 14(SP)
                  "PUSH 14(R6)\n\t"  // R7 is at 12(SP)
                  "PUSH 12(R6)\n\t"  // R6 is at 10(SP)
                  "PUSH 10(R6)\n\t"  // R5 is at 8(SP)
                  "PUSH 8(R6)\n\t"   // R4 is at 6(SP)
                  // skip R3 (CG)
                  "PUSH 6(R6)\n\t"   // R2 is at 4(SP)
                  "PUSH 4(R6)\n\t"   // R1 is at 2(SP)
                  "PUSH 2(R6)");     // R0 is at 0(SP)

    /* finally, overwrite the register file with the saved values from the
     * stack!  don't do the PC yet! */
    asm volatile ("MOV 28(R1), R15\n\t"
                  "MOV 26(R1), R14\n\t"
                  "MOV 24(R1), R13\n\t"
                  "MOV 22(R1), R12\n\t"
                  "MOV 20(R1), R11\n\t"
                  "MOV 18(R1), R10\n\t"
                  "MOV 16(R1), R9\n\t"
                  "MOV 14(R1), R8\n\t"
                  "MOV 12(R1), R7\n\t"
                  "MOV 10(R1), R6\n\t"
                  "MOV  8(R1), R5\n\t"
                  "MOV  6(R1), R4\n\t"
                  "MOV  4(R1), R2\n\t"
                  /* now set the stack pointer -- don't use any more locals from
                   * here to when we restore the PC */
                  "MOV  2(R1), R1");

    j = MEMREF_UINT(baseaddr + BUNDLE_SIZE_HEADER);
    asm volatile ("MOV %0, R0" ::"m"(j)); // implicit jump ... restored!
}

#ifdef MEMENTOS_TIMER
void __mementos_setup_timer (void) {
    TACCTL0 = CCIE; // CCR0 interrupt enabled
    TACCR0 = TIMER_INTERVAL;
    TACTL = TASSEL_2 + MC_1; // SMCLK, up mode
    __bis_status_register(GIE);
    ok_to_checkpoint = 0;
}
#endif

#ifdef MEMENTOS_TIMER
void Timer_A (void) {
    //__mementos_checkpoint();
    ok_to_checkpoint = 1;
    TACCR0 = TIMER_INTERVAL;
}
#endif
