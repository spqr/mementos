__attribute__((section(".init9"), aligned(2)))
int main (void) {
#ifdef MEMENTOS_TIMER
    __mementos_setup_timer();
#endif // MEMENTOS_TIMER

#if defined(MEMENTOS_HARDWARE) && defined(MEMENTOS_LOGGING)
    /* set up pins for event logging.  why can't we just use a contiguous range
     * like P3.0-4?  because some of those pins are hardwired to peripherals on
     * the WISP.  so we scavenge 4+1 pins from various ports according to
     * http://wisp.wikispaces.com/HW+Feature+Set */
    P1DIR |= BIT4; // P1.4
    P2DIR |= BIT3; // P2.3
    P3DIR |= BIT0 | BIT4 | BIT5; // P3.{0,4,5}
#endif // MEMENTOS_HARDWARE && MEMENTOS_LOGGING

    __mementos_log_event(MEMENTOS_STATUS_HELLO);

    // do any necessary energy-intensive cleanup tasks
    __mementos_atboot_cleanup();

    i = __mementos_find_active_bundle();

    if (__mementos_bundle_in_range(i)) {
        /* there's an active bundle inside one of the reserved segments */
        __mementos_inactive_cleanup(i);
        __mementos_restore(i);
        return 13; // shouldn't happen!  return 13 indicates bad luck.
    }

    /* invoke the original program */
    __mementos_log_event(MEMENTOS_STATUS_STARTING_MAIN);
    i = _old_main();
    __mementos_log_event(MEMENTOS_STATUS_PROGRAM_COMPLETE);

    return i;
}
