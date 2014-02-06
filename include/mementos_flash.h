#define MAINMEM_SEGSIZE 512

void __mementos_checkpoint (void);
void __mementos_restore (unsigned long);
unsigned int __mementos_locate_next_bundle (unsigned int);
unsigned int __mementos_find_active_bundle (void);
void __mementos_atboot_cleanup (void);
void __mementos_inactive_cleanup (unsigned int);
void __mementos_erase_segment (unsigned int);
void __mementos_mark_segment_erase (unsigned int);
unsigned int __mementos_segment_is_empty (unsigned int);
unsigned int __mementos_segment_marked_erase (unsigned int);
unsigned int __mementos_bundle_in_range (unsigned int);

#define REGISTER_BYTES (sizeof(unsigned int))
#define BUNDLE_SIZE_REGISTERS 30 // (15 * REGISTER_BYTES)
