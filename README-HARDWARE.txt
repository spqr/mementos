NOTE: these instructions apply only to the DL WISP 4.1 -- see
http://github.com/wisp/dlwisp41.

For logging (make HARDWARE=1 LOGGING=1), we use these pins on the DL WISP 4.1:
 > P1.4
 > P2.3
 > P3.{0,4,5}

If you're connecting the WISP to an outboard Olimex MSP430F2131 board running
monitoring code from ../simplemonitor, wire them together as follows:

 WISP pin    <->    Olimex pin    Notes
 1.4                1.0           (clock)
 2.3                2.0           LSB; see mementos.c:__mementos_log_event()
 3.0                2.1
 3.4                2.2
 3.5                2.3           MSB

# vim:tw=80 et
