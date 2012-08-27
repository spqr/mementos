#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(__clang__)
#define MEMENTOS_MAIN_ATTRIBUTES \
    __attribute__((section(".init9"), aligned(2)))
#elif defined(__MSPGCC__)
#define MEMENTOS_MAIN_ATTRIBUTES \
    __attribute__((hosted))
#endif // __clang__

#ifdef __MSPGCC__
__attribute__((section(".init9"), p2align(1,0)))
void __mementos_jump_to_main__ (void) { (void)main(); }
#endif // __MSPGCC__

#define MEMREF(x) (*((unsigned int*)(x)))

#endif // __COMMON_H__
