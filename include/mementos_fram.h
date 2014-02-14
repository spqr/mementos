#define FRAM_FIRST_BUNDLE_SEG  0xE000 /* XXX hack */
/* #define FRAM_FIRST_BUNDLE_SEG (FRAM_END+1 - (2*(TOPOFSTACK+1 - STARTOFDATA))) */
#define FRAM_SECOND_BUNDLE_SEG (FRAM_FIRST_BUNDLE_SEG + (TOPOFSTACK+1 - STARTOFDATA))

#define ACTIVE_BUNDLE_PTR (FRAM_FIRST_BUNDLE_SEG - (sizeof(unsigned long)))

void __mementos_checkpoint (void);
void __mementos_restore (unsigned long);
unsigned long __mementos_locate_next_bundle (void);
unsigned long __mementos_find_active_bundle (void);
void __mementos_fram_clear(unsigned long);
unsigned int __mementos_bundle_in_range (unsigned long);

#define REGISTER_BYTES (sizeof(unsigned long)) // bytes in a register
#define BUNDLE_SIZE_REGISTERS 60 // (15 * REGISTER_BYTES)
