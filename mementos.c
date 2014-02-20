/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h> // XXX hack

// extern int _old_main (void);
unsigned int baseaddr;
unsigned int i, j, k;
unsigned int tmpsize;
// unsigned int interrupts_enabled;
#ifdef MEMENTOS_TIMER
bool ok_to_checkpoint;
#endif // MEMENTOS_TIMER

void __mementos_restore (unsigned int b) {
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
    //              GS SS 
    /*baseaddr+0-->[--|--|      //global size and stack size
      baseaddr+4--> R0|R1|R2|R4 //note: there is no r3
                    R5|R6|R7|R8
                    R9|RA|RB|RC
                    RD|RE|RF
      baseaddr+34-->[--... SS bytes ...--] //stack
   baseaddr+34+SS-->[0xBEAD];
 baseaddr+34+SS+1-->{business as usual...}

    */
    /* restore the stack by walking from the top to the bottom of the stack
     * portion of the checkpoint */
    tmpsize = MEMREF_UINT(baseaddr + 2); // stack size
    for (i = 0; i < tmpsize; i += 2) {
        /* summary:
        MEMREF(TOPOFSTACK - i) = MEMREF(baseaddr + 30 + tmpsize - i);
        */

        // j = TOPOFSTACK - i - 2;
        asm volatile("MOV #" xstr(TOPOFSTACK) ", %0" :"=m"(j));
        asm volatile("SUB %0, %1" :"=m"(i) :"m"(j));
        asm volatile("DECD.W %0" ::"m"(j));

        // k = baseaddr + BUNDLE_SIZE_REGISTERS + 4 + tmpsize - i;
        asm volatile("MOV %1, %0" :"=m"(k) :"m"(baseaddr));
        asm volatile("ADD #" xstr(BUNDLE_SIZE_REGISTERS) ", %0" ::"m"(k));
        asm volatile("ADD #4, %0" ::"m"(k));
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

    // grab the size of the size of the globals we'll have to restore
    asm volatile("MOV %0, R7" ::"m"(tmpsize));  // R7(stacksize) = tmpsize
    asm volatile("MOV %0, R6" ::"m"(baseaddr)); // R6(baseaddr)  = baseaddr
    asm volatile("MOV @R6, R8");                // R8(globalsize) =
    asm volatile("AND #255, R8");               //   MEMREF(baseaddr) & 0x00FF

    asm volatile("CLR.W R9");                    // R9(i) = 0 // induction var
    asm volatile("rdloop:");                     // will jump back up here
    asm volatile("CMP R8, R9");                // if (i >= globalsize)
    asm volatile("JC afterrd");                  //   <stop looping>

    // copy one word at a time from checkpoint to data segment
    asm volatile("MOV R6, R10");               // R10 = baseaddr
    asm volatile("ADD #34, R10");              // this constant is ugly now, but 34 is SS + GS + REGS 
    asm volatile("ADD R7, R10");               // + stacksize
    asm volatile("ADD R9, R10");               // + i, which is the counter for which glob we're on
    asm volatile("MOV 0(R10), " xstr(STARTOFDATA) "(R9)"); // MEMREF(STARTOFDATA+i(R9)) =
                                                         //    MEMREF(R10)
    asm volatile("INCD R9");                     // i += 2
    asm volatile("JMP rdloop");                  // to beginning of loop

    asm volatile("afterrd:");                    // jump here when done

    /* set baseaddr back to whatever it was -- BRANDON: why???*/
    asm volatile("MOV R6, %0" :"=m"(baseaddr));

    /* finally, restore all the registers, starting at R15 and counting down to
     * R0/PC.  setting R0/PC is an implicit jump, so we have to do it last. */

    /* j = <PC to restore> (note: R6 still contains baseaddr) */
    asm volatile ("MOV 4(R6), %0" :"=m"(j));

    /* set the SP first, so we can PUSH stuff; we'll reset it later */
    asm volatile ("MOV 6(R6), R1");
    /* now push the saved register values onto the stack (R6=baseaddr) */
    
    asm volatile ("MOV 32(R6), R15");  // R15
    asm volatile ("MOV 30(R6), R14");  // R14
    asm volatile ("MOV 28(R6), R13");  // R13
    asm volatile ("MOV 26(R6), R12");  // R12
    asm volatile ("MOV 24(R6), R11");  // R11
    asm volatile ("MOV 22(R6), R10");  // R10
    asm volatile ("MOV 20(R6), R9");  // R9
    asm volatile ("MOV 18(R6), R8");  // R8
    asm volatile ("MOV 16(R6), R7");  // R7
    asm volatile ("MOV 12(R6), R5");  // R5
    asm volatile ("MOV 10(R6), R4");  // R4
    // skip R3 (CG)
    asm volatile ("MOV 8(R6), R2");   // R2
    asm volatile ("MOV 6(R6), R1");   // R1

    /*Why do R6 and R0 this way?  Because baseaddr is
      in R6.  That means we need to save R0 before over
      writing R6 with its value from the checkpoint.
   
      Then, after we recover R6, we can pop R0 from the stack into its home,
      redirecting control flow to the point in the checkpoint.
    */
    asm volatile ("PUSH 4(R6)");  // R6
    asm volatile ("MOV 14(R6), R6");  // R6
    asm volatile ("POP R0");   // R0
    

    /*asm volatile ("PUSH 32(R6)");  // R15
    asm volatile ("PUSH 30(R6)");  // R14
    asm volatile ("PUSH 28(R6)");  // R13
    asm volatile ("PUSH 26(R6)");  // R12
    asm volatile ("PUSH 24(R6)");  // R11
    asm volatile ("PUSH 22(R6)");  // R10
    asm volatile ("PUSH 20(R6)");  // R9
    asm volatile ("PUSH 18(R6)");  // R8
    asm volatile ("PUSH 16(R6)");  // R7
    asm volatile ("PUSH 14(R6)");  // R6
    asm volatile ("PUSH 12(R6)");  // R5
    asm volatile ("PUSH 10(R6)");  // R4
    // skip R3 (CG)
    asm volatile ("PUSH 8(R6)");   // R2
    asm volatile ("PUSH 6(R6)");   // R1
    asm volatile ("PUSH 4(R6)");   // R0*/

    /* finally, overwrite the register file with the saved values from the
     * stack!  don't do the PC yet! */
    /*asm volatile ("MOV 30(R1), R15");
    asm volatile ("MOV 28(R1), R14");
    asm volatile ("MOV 26(R1), R13");
    asm volatile ("MOV 24(R1), R12");
    asm volatile ("MOV 22(R1), R11");
    asm volatile ("MOV 20(R1), R10");
    asm volatile ("MOV 18(R1), R9");
    asm volatile ("MOV 16(R1), R8");
    asm volatile ("MOV 14(R1), R7");
    asm volatile ("MOV 12(R1), R6");
    asm volatile ("MOV 10(R1), R5");
    asm volatile ("MOV 8(R1), R4");
    asm volatile ("MOV  6(R1), R2");*/
    /* now set the stack pointer -- don't use any more locals from
    * here to when we restore the PC */
    //asm volatile ("MOV  4(R1), R1");

    //j = MEMREF_UINT(baseaddr + BUNDLE_SIZE_HEADER);
    //asm volatile ("MOV %0, R0" ::"m"(j)); // implicit jump ... restored!
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
