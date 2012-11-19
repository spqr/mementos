#include "common.h"
#include <stdio.h>

/*
clang -ccc-host-triple msp430-elf -ccc-driver-config ../llvm/testcases/config.json -emit-llvm globals.c -c
opt -stats -load $LLVMBUILD/lib/libMementosAnalysis.dylib -size-globals -load $LLVMBUILD/lib/libMementosTransforms.dylib -insert-trigger-points -add-global-size-global -o /dev/null -debug globals.o
*/

volatile int argh = 0xbe;
static int fuzz = 0xc0;
long beep = 0x55;

int blargh (int x) {
    return (x > argh) ? x : argh;
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    char a = 7;
    int i;
    for (i = 0; i < a; ++i) --argh;
    if (blargh((int)a) > argh) {
        return (int)a + 1;
    } else {
        return a * fuzz;
    }
}
