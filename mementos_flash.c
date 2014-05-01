/* See license.txt for licensing information. */

#include <mementos.h>
#include <msp430builtins.h>

#ifndef MEMENTOS_FLASH
#error Inappropriate use of mementos_flash.c without MEMENTOS_FLASH defined
#endif

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
#ifdef MEMENTOS_VOLTAGE_CHECK
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
#endif // MEMENTOS_VOLTAGE_CHECK
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

void __mementos_atboot_cleanup (void) {
    if (__mementos_segment_marked_erase(FIRST_BUNDLE_SEG))
        __mementos_erase_segment(FIRST_BUNDLE_SEG);
    if (__mementos_segment_marked_erase(SECOND_BUNDLE_SEG))
        __mementos_erase_segment(SECOND_BUNDLE_SEG);
}

unsigned int __mementos_bundle_in_range (unsigned int bun_addr) {
    return ((bun_addr >= FIRST_BUNDLE_SEG) &&
            (bun_addr < SECOND_BUNDLE_SEG + MAINMEM_SEGSIZE));
}

void __mementos_inactive_cleanup (unsigned int active_bundle_addr) {
    if (active_bundle_addr < SECOND_BUNDLE_SEG) {
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
}

unsigned int __mementos_force_free (void) {
    /* erase a segment (but only one) if we can */
    if (__mementos_segment_marked_erase(FIRST_BUNDLE_SEG)) {
        __mementos_erase_segment(FIRST_BUNDLE_SEG);
        return FIRST_BUNDLE_SEG;
    } else if (__mementos_segment_marked_erase(SECOND_BUNDLE_SEG)) {
        __mementos_erase_segment(SECOND_BUNDLE_SEG);
        return SECOND_BUNDLE_SEG;
    } else {
        // neither segment marked for erasure; choose the first segment
        __mementos_mark_segment_erase(FIRST_BUNDLE_SEG);
        __mementos_mark_segment_erase(SECOND_BUNDLE_SEG);
        __mementos_erase_segment(FIRST_BUNDLE_SEG);
        return FIRST_BUNDLE_SEG;
    }
}
