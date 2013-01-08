#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(MEMENTOS_LATCH) || defined(MEMENTOS_RETURN) || \
    defined(MEMENTOS_TIMER) || defined(MEMENTOS_ORACLE)
#define MEMENTOS
#endif

#ifdef __clang__
#ifndef MEMENTOS
/* compiling with clang but not against mementos; put testcase's main() in
 * .init9 so it runs at boot */
#define MEMENTOS_MAIN_ATTRIBUTES \
    __attribute__((section(".init9"), aligned(2)))
#else
/* compiling with mementos; don't put testcase's main() in .init9 because
 * mementos's own main() (which wraps testcase's main()) belongs there */
#define MEMENTOS_MAIN_ATTRIBUTES
#endif // MEMENTOS
#endif // __clang__

#ifdef __MSPGCC__
#define MEMENTOS_MAIN_ATTRIBUTES __attribute__((hosted))

/* wrap main() to make the call/return from main() explicit/profilable */
__attribute__((section(".init9"), p2align(1,0)))
void __mementos_jump_to_main__ (void) { (void)main(); }
#endif // __MSPGCC__

#define MEMREF(x) (*((unsigned int*)(x)))

#endif // __COMMON_H__
