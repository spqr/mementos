#ifdef __MSPGCC__
#  include <msp430.h>
#elif defined(__clang__)
#  include <msp430fr5969.h>
#else
#  error Unsupported compiler; use mspgcc or clang.
#endif

#define MEMENTOS_FRAM

#define TOPOFSTACK 0x2400
#define STARTOFDATA 0x1C00
#define FRAM_END 0xFF7F /*BRANDON: hacked to accomodate mov vs. movx bug*/
