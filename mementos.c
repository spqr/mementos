/*
Copyright (c) 2011, University of Massachusetts Amherst
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. Neither the name of the University of Massachusetts Amherst nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written
    permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <mementos.h>
#include <msp430builtins.h> // XXX hack

extern int _old_main (void);
unsigned int baseaddr;
unsigned int i, j, k;
unsigned int tmpsize;
// unsigned int interrupts_enabled;
#ifdef MEMENTOS_TIMER
bool ok_to_checkpoint;
#endif // MEMENTOS_TIMER

void __mementos_checkpoint (void) {
    /* Size of globals in bytes.  Count this far from the beginning of RAM to
     * capture globals.  Comes from the AddGlobalSizeGlobal pass. */
    extern int GlobalAllocSize;
#   define OFFSET  0 /* size (in bytes) of this function's locals */

    /*
    // disable interrupts; getting interrupted might torpedo the process
    asm volatile ("MOV R2, &%0\n\t"
                  "DINT"
                  :"=m"(interrupts_enabled));
    */

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
    asm volatile ("MOV 26(R1), &%0" :"=m"(j)); // j = SP
    /* j now contains the pre-call value of the stack pointer */

    baseaddr = __mementos_locate_next_bundle(j);
    if (baseaddr == 0xFFFFu) { // oh no, nowhere to put this bundle
        /* erase a segment (but only one) if we can */
        if (__mementos_segment_marked_erase(FIRST_BUNDLE_SEG)) {
            __mementos_erase_segment(FIRST_BUNDLE_SEG);
            baseaddr = FIRST_BUNDLE_SEG;
        } else if (__mementos_segment_marked_erase(SECOND_BUNDLE_SEG)) {
            __mementos_erase_segment(SECOND_BUNDLE_SEG);
            baseaddr = SECOND_BUNDLE_SEG;
        } else {
            /* oh god, neither segment was marked for erasure */
            __mementos_mark_segment_erase(FIRST_BUNDLE_SEG);
            __mementos_mark_segment_erase(SECOND_BUNDLE_SEG);
            asm volatile("ADD #30, R1\n\t" // make up for those pushes
                         "RET"); // bail
        }
    }

    // make flash writable
    asm volatile ("DINT");
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + WRT;

    /********** phase #0: save size header (2 bytes) **********/
    asm volatile ("PUSH R12");
    asm volatile ("PUSH R13");

    // compute size of stack (in bytes) into R13
    asm volatile ("MOV #" xstr(TOPOFSTACK) ", R13");
    asm volatile ("SUB &%0, R13" ::"m"(j)); // j == old stack pointer

    // write size of stack (R13) to high byte at baseaddr
    asm volatile ("MOV &%0, R12" ::"m"(baseaddr));
    asm volatile ("MOV.B R13, 1(R12)");

    // store GlobalAllocSize into R13, round it up to the next word boundary
    asm volatile ("MOV &%0, R13" ::"m"(GlobalAllocSize));
    asm volatile ("INC R13");
    asm volatile ("AND #0xFE, R13");

    // write GlobalAllocSize to low byte at baseaddr
    asm volatile ("MOV.B R13, 0(R12)");

    asm volatile ("POP R13");
    asm volatile ("POP R12");

    /********** phase #1: checkpoint registers. **********/
    asm volatile ("MOV &%0, R14" ::"m"(baseaddr));
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
        MEMREF((baseaddr + BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS + j-(TOPOFSTACK-i))) = MEMREF(i);
        i += 2;
    }

    /* checkpoint as much of the data segment as is necessary */
    j += BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS;
    for (i = STARTOFDATA; i < STARTOFDATA+ROUND_TO_NEXT_EVEN(GlobalAllocSize);
            i += 2, j += 2) {
        MEMREF(baseaddr + j) = MEMREF(i);
    }
    // write the magic number
    MEMREF(baseaddr + j) = MEMENTOS_MAGIC_NUMBER;
    __mementos_log_event(MEMENTOS_STATUS_COMPLETED_CHECKPOINT);

    /* success!  if this was the first in a segment, mark the other segment for
     * erasure because we've superseded all its checkpoints with this one */
    if ((baseaddr == FIRST_BUNDLE_SEG) || (baseaddr == SECOND_BUNDLE_SEG)) {
        j = (baseaddr < SECOND_BUNDLE_SEG) ? SECOND_BUNDLE_SEG : FIRST_BUNDLE_SEG;
        if (!__mementos_segment_is_empty(j))
            __mementos_mark_segment_erase(j);
    }

    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
    asm volatile ("EINT");

#ifdef MEMENTOS_TIMER
    // CCR0 = TIMER_INTERVAL; // XXX choose a good interval -- what are the units?
    ok_to_checkpoint = 0; // not OK to checkpoint until next timer fire
#endif

    /*
    // reenable interrupts -- but only if they were enabled when we started
    if (interrupts_enabled & 0x8) // GIE bit in SR/R2 (see MSP430 manual)
      asm volatile ("EINT");
    */
}

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

    /* restore the stack by walking from the top to the bottom of the stack
     * portion of the checkpoint */
    tmpsize = MEMREF(baseaddr) >> 8; // stack size
    for (i = 0; i < tmpsize; i += 2) {
        /* summary:
        MEMREF(TOPOFSTACK - i) = MEMREF(baseaddr + 30 + tmpsize - i);
        */

        // j = TOPOFSTACK - i - 2;
        asm volatile("MOV #" xstr(TOPOFSTACK) ", &%0" :"=m"(j));
        asm volatile("SUB &%0, &%1" :"=m"(i) :"m"(j));
        asm volatile("DECD.W &%0" ::"m"(j));

        // k = baseaddr; k += 30; k += tmpsize; k -= i;
        asm volatile("MOV &%1, &%0" :"=m"(k) :"m"(baseaddr));
        asm volatile("ADD #" xstr(BUNDLE_SIZE_REGISTERS) ", &%0" ::"m"(k));
        asm volatile("ADD &%1, &%0" :"=m"(k) :"m"(tmpsize));
        asm volatile("SUB &%1, &%0" :"=m"(k) :"m"(i));

        // MEMREF(j) = MEMREF(k);
        asm volatile("MOV &%0, R7" ::"m"(k));
        asm volatile("MOV &%0, R8" ::"m"(j));
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
    asm volatile("MOV &%0, R7" ::"m"(tmpsize));  // R7(stacksize) = tmpsize
    asm volatile("MOV &%0, R6" ::"m"(baseaddr)); // R6(baseaddr)  = baseaddr
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
    asm volatile("MOV R6, &%0" :"=m"(baseaddr));

    /* finally, restore all the registers, starting at R15 and counting down to
     * R0/PC.  setting R0/PC is an implicit jump, so we have to do it last. */

    /* j = <PC to restore> (note: R6 still contains baseaddr) */
    asm volatile ("MOV 2(R6), &%0" :"=m"(j));

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

    /* no need to re-enable interrupts; restoring R2 does the right thing
    asm volatile ("EINT\n\t" // re-enable interrupts at the last opportunity */
    j = MEMREF(baseaddr + BUNDLE_SIZE_HEADER);
    asm volatile ("MOV &%0, R0" ::"m"(j)); // implicit jump ... restored!
}

unsigned int __mementos_segment_is_empty (unsigned int addr) {
    unsigned int a;
#define STRIDE (BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS)
    for (a = addr; a < (addr+MAINMEM_SEGSIZE); a += STRIDE) {
        if (MEMREF(a) != 0xFFFFu) return 0;
    }
    return 1;
}

unsigned int __mementos_segment_marked_erase (unsigned int addr) {
    return ((MEMREF(addr+2) == 0x0000u)
            && (MEMREF(addr+MAINMEM_SEGSIZE-2) == 0x0000u));
}

void __mementos_mark_segment_erase (unsigned int addr) {
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + WRT;
    MEMREF(addr+2) = 0x0000u;
    MEMREF(addr+MAINMEM_SEGSIZE-2) = 0x0000u;
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

/* rules:
 *  - don't put a bundle at the beginning of a non-erased segment
 */
unsigned int __mementos_locate_next_bundle (unsigned int sp /* hack */) {
    extern int GlobalAllocSize;
    unsigned int size, b, bsize, next;

    /* how big is the bundle we'd like to store? */
    size =
        BUNDLE_SIZE_HEADER // bytes for the bundle header
        + BUNDLE_SIZE_REGISTERS // bytes for the register portion
        + (TOPOFSTACK - sp) // bytes for the stack portion
        + ROUND_TO_NEXT_EVEN(GlobalAllocSize) // bytes for the dataseg portion
        + BUNDLE_SIZE_MAGIC; // bytes for the magic number

    /* where does the currently active bundle start? */
    baseaddr = __mementos_find_active_bundle();
    if (baseaddr == 0xFFFFu) {
        if (__mementos_segment_is_empty(FIRST_BUNDLE_SEG))
            return FIRST_BUNDLE_SEG;
        else if (__mementos_segment_is_empty(SECOND_BUNDLE_SEG))
            return SECOND_BUNDLE_SEG;

        return 0xFFFFu;
    }

    /* grab the size of the currently active bundle */
    bsize =
        BUNDLE_SIZE_HEADER // bytes for the bundle header
        + BUNDLE_SIZE_REGISTERS // bytes for the register portion
        + (MEMREF(baseaddr) >> 8) // for the stack portion
        + (MEMREF(baseaddr) & 0xFF) // for the dataseg portion
        + BUNDLE_SIZE_MAGIC; // bytes for the magic number

    next = baseaddr + bsize; // next byte after currently active bundle

    /* if there's a bundle header at the next location, we've encountered an
     * aborted checkpoint and need to skip over it.  this could happen multiple
     * times if we have already skipped over failed checkpoints but then failed
     * again. */
    while (MEMREF(next) != 0xFFFFu) {
        bsize =
            BUNDLE_SIZE_HEADER // bytes for the bundle header
            + BUNDLE_SIZE_REGISTERS // bytes for the register portion
            + (MEMREF(next) >> 8) // for the stack portion
            + (MEMREF(next) & 0x00FFu) // for the dataseg portion
            + BUNDLE_SIZE_MAGIC; // bytes for the magic number
        next += bsize;

        if (next > (SECOND_BUNDLE_SEG + MAINMEM_SEGSIZE)) { // out of bounds
            /* next bundle should go in whichever segment doesn't contain the
             * current bundle */
            next = (baseaddr >= SECOND_BUNDLE_SEG)
                ? FIRST_BUNDLE_SEG
                : SECOND_BUNDLE_SEG;
            if (!__mementos_segment_is_empty(next)) {
                if (!__mementos_segment_marked_erase(next))
                    __mementos_mark_segment_erase(next);
                /* the next location isn't empty, but it's marked for erasure,
                 * which means a call to locate_bundle after erasing it will
                 * correctly locate the bundle there.  but since it's not
                 * immediately usable we must report as much. */
                return 0xFFFFu;
            }
        }
    }

    if (next < SECOND_BUNDLE_SEG) { // bundle starts in first segment
        if ((next + size) > SECOND_BUNDLE_SEG) {
            // overlaps with second segment, promote it to there
            if (__mementos_segment_is_empty(SECOND_BUNDLE_SEG)) {
                next = SECOND_BUNDLE_SEG;
            } else {
                __mementos_mark_segment_erase(SECOND_BUNDLE_SEG);
                return 0xFFFFu;
            }
        }
    } else { // bundle starts in second bundle segment
        if ((next + size) > (SECOND_BUNDLE_SEG + MAINMEM_SEGSIZE)) {
            // overruns end of second segment, put bundle in first segment
            if (__mementos_segment_is_empty(FIRST_BUNDLE_SEG)) {
                next = FIRST_BUNDLE_SEG;
            } else {
                __mementos_mark_segment_erase(FIRST_BUNDLE_SEG);
                return 0xFFFFu;
            }
        }
    }
    return next;
}

/* walks through the reserved areas looking for the most recent complete
 * checkpoint bundle.  returns either the address of the active bundle or
 * 0xFFFFu if no active bundle is found. */
unsigned int __mementos_find_active_bundle (void) {
    unsigned int bun = FIRST_BUNDLE_SEG;
    unsigned int pri_segment, alt_segment;
    unsigned int magic, endloc;
    unsigned int candidate = 0xFFFFu; // current candidate for active bundle
    __mementos_log_event(MEMENTOS_STATUS_FINDING_BUNDLE);
    if (__mementos_segment_is_empty(FIRST_BUNDLE_SEG) ||
            __mementos_segment_marked_erase(FIRST_BUNDLE_SEG)) {
        pri_segment = SECOND_BUNDLE_SEG;
        alt_segment = FIRST_BUNDLE_SEG;

searchbundle:
        // alt_segment does not contain an active bundle, so look in pri_segment
        bun = pri_segment;
        do {
            /* on this loop's first iteration, candidate == 0xFFFFu; if there's
             * no bundle at pri_segment, there's no active bundle, so
             * return candidate */
            if (MEMREF(bun) == 0xFFFFu) {
                /* found a blank where a bundle header should be, so we've hit
                 * the end of bundles in this segment; done looking. */
                break;
            }

            /* compute the location of the canary.  if the word at this location
             * is equal to MEMENTOS_MAGIC_NUMBER, then this is a valid
             * checkpoint bundle. */
            endloc =
                bun                     // start of bundle
                + BUNDLE_SIZE_HEADER
                + BUNDLE_SIZE_REGISTERS
                + (MEMREF(bun) >> 8)    // size of stack in bytes
                + (MEMREF(bun) & 0xFF); // size of data segment in bytes

            magic = MEMREF(endloc);
            if (magic == MEMENTOS_MAGIC_NUMBER) {
                /* found a valid bundle */
                candidate = bun;
            } else {
                /* found an abortive bundle.  since we never try to write a
                 * bundle after an abortive bundle (if we did, we wouldn't be
                 * able to find it using our walking technique), this segment is
                 * no longer useful, so we mark it for erasure */

                /* if the segment starts off with a botched checkpoint, the
                 * whole thing's shot and we should erase it when we get a
                 * chance */
                // if (bun == pri_segment)
                    __mementos_mark_segment_erase(pri_segment);
                /* else consider the previous bundle the active one (candidate
                 * points at the most recently validated bundle) */

                break;
            }
            bun = endloc + 2 /* skip over 2-byte magic number */;
        } while (bun < (pri_segment + MAINMEM_SEGSIZE));

        if (candidate != 0xFFFFu) {
            // don't need the other segment's data anymore
            if (!__mementos_segment_is_empty(FIRST_BUNDLE_SEG)) {
                __mementos_mark_segment_erase(FIRST_BUNDLE_SEG);
            }
        }

        __mementos_log_event(MEMENTOS_STATUS_DONE_FINDING_BUNDLE);
        return candidate;
    }

    // if we reach this point, the first bundle may contain an active bundle
    pri_segment = FIRST_BUNDLE_SEG;
    alt_segment = SECOND_BUNDLE_SEG;
    goto searchbundle;
}

/* erases the segment containing the address addr_in_segment */
void __mementos_erase_segment (unsigned int addr_in_segment) {
    asm volatile ("DINT");
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + ERASE;
    MEMREF(addr_in_segment) = 0;
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
    asm volatile ("EINT"); // XXX should probably *conditionally* reenable here
}


__attribute__((section(".init9"), aligned(2)))
int main (void) {
#ifdef MEMENTOS_TIMER
    TACCTL0 = CCIE; // CCR0 interrupt enabled
    TACCR0 = TIMER_INTERVAL;
    TACTL = TASSEL_2 + MC_1; // SMCLK, up mode
    __bis_status_register(GIE);
    ok_to_checkpoint = 0;
#endif

#if defined(MEMENTOS_HARDWARE) && defined(MEMENTOS_LOGGING)
    /* set up pins for event logging.  why can't we just use a contiguous range
     * like P3.0-4?  because some of those pins are hardwired to peripherals on
     * the WISP.  so we scavenge 4+1 pins from various ports according to
     * http://wisp.wikispaces.com/HW+Feature+Set */
    P1DIR |= BIT4; // P1.4
    P2DIR |= BIT3; // P2.3
    P3DIR |= BIT0 | BIT4 | BIT5; // P3.{0,4,5}
#endif // MEMENTOS_HARDWARE && MEMENTOS_LOGGING

    __mementos_log_event(MEMENTOS_STATUS_HELLO);

    /* super-quick check at boot time; may result in the erasure of at most one
     * flash segment */
    if (__mementos_segment_marked_erase(FIRST_BUNDLE_SEG))
        __mementos_erase_segment(FIRST_BUNDLE_SEG);
    if (__mementos_segment_marked_erase(SECOND_BUNDLE_SEG))
        __mementos_erase_segment(SECOND_BUNDLE_SEG);

    i = __mementos_find_active_bundle();
    if ((i >= FIRST_BUNDLE_SEG) && (i < SECOND_BUNDLE_SEG + MAINMEM_SEGSIZE)) {
        /* there's an active bundle inside one of the reserved segments */
        if (i < SECOND_BUNDLE_SEG) {
            /* bundle is in first segment, so erase the second segment if
             * necessary */
            if (!__mementos_segment_is_empty(SECOND_BUNDLE_SEG))
                __mementos_erase_segment(SECOND_BUNDLE_SEG);
        } else {
            /* bundle is in second segment, so erase the first segment if
             * necessary */
            if (!__mementos_segment_is_empty(FIRST_BUNDLE_SEG))
                __mementos_erase_segment(FIRST_BUNDLE_SEG);
        }

        __mementos_restore(i);
        return 13; // shouldn't happen!  return 13 indicates bad luck.
    }

    /* invoke the original program */
    __mementos_log_event(MEMENTOS_STATUS_STARTING_MAIN);
    i = _old_main();
    __mementos_log_event(MEMENTOS_STATUS_PROGRAM_COMPLETE);

    return i;
}

#ifdef MEMENTOS_HARDWARE
/* Take a single ADC reading to determine capacitor voltage.
 * Code adapted from:
 *   - Intel WISP 4.1 firmware
 *   - 'blinky.c' from http://wisp.wikispaces.com/WISP+code+examples
 * Further advice from
 *   - http://wisp.wikispaces.com/ManagingPower
 * THIS CODE IS INTEL DLWISP 4.1 SPECIFIC! */
unsigned int __mementos_voltage_check (void) {
    unsigned char i = 0;

    __mementos_log_event(MEMENTOS_STATUS_CHECKING_VOLTAGE);

    /* set up the ADC (a la blinky.c) */
    ADC10CTL0 &= ~ENC;
    ADC10CTL1 = INCH_VSENSE_IN + ADC10DIV_3;
    ADC10CTL0 = ADC10SHT_3 + ADC10ON + SREF_1 + REFON;// + REF2_5V;
    P3DIR = VSENSE_POWER;
    P3OUT |= VSENSE_POWER; // turn on the WISP's voltage sensing circuit

    for (i = 0; i < 50; ++i);

    ADC10CTL0 |= ENC + ADC10SC; // start the ADC sampling
    while (ADC10CTL1 & ADC10BUSY); // wait while it takes a sample
    ADC10CTL0 = 0; // turn off ADC
    P3OUT &= ~VSENSE_POWER; // turn off voltage sensing circuit

    __mementos_log_event(MEMENTOS_STATUS_DONE_CHECKING_VOLTAGE);

    /* ADC10MEM now contains 1024 * ((Vout/3) / 2.5V);
     * Vout = 3 * 2.5V * ADC10MEM / 1024;
     * scale up the 10-bit result to a full unsigned int */
    return ADC10MEM << 6;
}
#endif // MEMENTOS_HARDWARE

#ifdef MEMENTOS_LOGGING
/* log a 4-bit event (lowest 4 bits of evt) to an external listening device.
 * see http://wisp.wikispaces.com/HW+Feature+Set to find free pins that aren't
 * hardwired to power-hungry peripherals.
 */
void __mementos_log_event (unsigned char evt) {
#ifdef MEMENTOS_HARDWARE
    P2OUT |= (evt & BIT0) << 3; // put bit 0/LSB of evt -> P2.3
    P3OUT |= (evt & BIT1) >> 1; // put bit 1 of evt     -> P3.0
    P3OUT |= (evt & BIT2) << 2; // put bit 2 of evt     -> P3.4
    P3OUT |= (evt & BIT3) << 2; // put bit 3 of evt     -> P3.5
    P1OUT |= BIT4; // raise the clock pin                  P1.4

    asm volatile ("NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n\t"
                  "NOP"); // wait a bit (xxx how much is enough?)

    // put the pins back down
    P1OUT &= ~BIT4;
    P2OUT &= ~BIT3;
    P3OUT &= ~(BIT0 | BIT4 | BIT5);
#else
    /* logging without hardware is currently a no-op XXX */
#endif // MEMENTOS_HARDWARE
}
#endif // MEMENTOS_LOGGING

#ifdef MEMENTOS_TIMER
void Timer_A (void) {
    //__mementos_checkpoint();
    ok_to_checkpoint = 1;
    TACCR0 = TIMER_INTERVAL;
}
#endif
