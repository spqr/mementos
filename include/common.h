#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(__MSPGCC__) || (defined(__MSP430__) && !defined(__clang__))
#if (__MSPGCC__ >= 20110706)
/* squash stuff that would put program in endless loop after main */
void _endless_loop__ (void) { }
void __ctors_end (void) { }
void _unexpected_ (void) { }
#else
/* squash stuff that would put program in endless loop after main */
void __stop_progExec__ (void) { }
#endif // __MSPGCC__
#endif // mspgcc

// from newmem.h
#define MEMREF(x) (*((unsigned int*)(x)))

#endif // __COMMON_H__
