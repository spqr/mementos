// #include "common.h"

int add(int a, int b) { return a+b; }
#ifdef __clang__
// __attribute__((section(".fini9"))) void donothing () { }
__attribute__((section(".init9")))
__attribute__((aligned(2)))
#endif // __clang __
int main (void) { return add(5, 6); }
