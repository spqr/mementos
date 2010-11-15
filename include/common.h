#ifndef __COMMON_H__

#if defined(__MSP430__) && !defined(__clang__) // mspgcc
/* if you don't substitute your own __stop_progExec__ function for mspgcc's
 * default, your program will loop forever when its work is done */
void __stop_progExec__ (void) {
}
#endif

// from newmem.h
#define MEMREF(x) (*((unsigned int*)(x)))

#endif // __COMMON_H__
