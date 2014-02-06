/* See license.txt for licensing information. */

#ifndef __MEMENTOS_H__
#define __MEMENTOS_H__

#if defined(__MSP430F2131__)
#  include <f2131.h>
#elif defined(__MSP430F2132__)
#  include <f2132.h>
#elif defined(__MSP430F1611__)
#  include <f1611.h>
#elif defined(__MSP430F2618__)
#  include <f2618.h>
#elif defined(__MSP430FR5969__)
#  include <fr5969.h>
#else
#  error Missing or unsupported chip.
#endif

#ifdef MEMENTOS_FRAM
#  include <mementos_fram.h>
#else
#  include <mementos_flash.h>
#endif

#define RAM_SIZE (TOPOFSTACK+1 - STARTOFDATA)

/* V_THRESH corresponding to voltage V == round(65536 * (V/3.0)/2.5) */
#define V_THRESH 23593 // 2.7 volts

/* silly string functions for stringifying #define'd constants -- don't ask */
#define xstr(s) str(s)
#define str(s) #s

#ifdef MEMENTOS_LOGGING
# define MEMENTOS_STATUS_STARTING_CHECKPOINT 0x1
# define MEMENTOS_STATUS_COMPLETED_CHECKPOINT 0x2
# define MEMENTOS_STATUS_STARTING_RESTORATION 0x4
# define MEMENTOS_STATUS_STARTING_MAIN 0x8
# define MEMENTOS_STATUS_FINDING_BUNDLE 0x9
# define MEMENTOS_STATUS_DONE_FINDING_BUNDLE 0xA
# define MEMENTOS_STATUS_HELLO 0xB
# define MEMENTOS_STATUS_CHECKING_VOLTAGE 0xC
# define MEMENTOS_STATUS_DONE_CHECKING_VOLTAGE 0xD
# define MEMENTOS_STATUS_PROGRAM_COMPLETE 0xE
void __mementos_log_event (unsigned char);
#else
# define __mementos_log_event(x)
#endif // MEMENTOS_LOGGING

#ifdef MEMENTOS_HARDWARE
unsigned int __mementos_voltage_check (void);
#define VOLTAGE_CHECK __mementos_voltage_check()
#else
/* 0x01C0 is an (apparently) unused addr in peripheral memory on MSP430F2132, so
 * our simulator (mspsim) tracks accesses to it and returns energy data when
 * it's read. */
extern volatile unsigned int VOLTAGE_CHECK asm("0x01C0");
#endif // MEMENTOS_HARDWARE

#define MEMENTOS_MAGIC_NUMBER 0xBEADu

#ifdef MEMENTOS_TIMER
void Timer_A (void) __attribute__((interrupt(12))); // 0xFFEC for F1611 XXX
#define TIMER_INTERVAL 20000
#endif

#define MEMREF_UINT(x) (*((unsigned int*)(x)))
#define MEMREF_ULONG(x) (*((unsigned long*)(x)))

#define BUNDLE_SIZE_HEADER 2     // stack size (1 byte) + dataseg size (1 byte)
#define BUNDLE_SIZE_MAGIC 2      // 2 bytes for MEMENTOS_MAGIC_NUMBER

// distances from start of bundle...
#define REGISTERS_OFFSET BUNDLE_SIZE_HEADER
#define STACK_OFFSET (BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS)

#define ROUND_TO_NEXT_EVEN(x) (((x)+1) & 0xFFFEu)

typedef unsigned char bool;

#endif /* __MEMENTOS_H__ */
// vim:ft=c
