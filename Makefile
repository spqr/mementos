#
### Mementos build system.
#
# To build against a <program>.c (default: sense.c), run:
#  make TARGET=program
# To build with hardware support, define HARDWARE:
#  make HARDWARE=1 [...]
# To build with hardware support *and* support for logging to an outboard
# device (recommended), run:
#  make HARDWARE=1 LOGGING=1 [...]
#

# conditional definitions
TARGET ?= samples/sense

include Makefile.common

.DUMMY: all clean gdbcommands install strip

ifeq ($(TARGET),sense)
 MCLIBS=-lm
endif

ifdef TIMER
override CFLAGS += -DMEMENTOS_TIMER
endif

ifdef HARDWARE
override CFLAGS += -DMEMENTOS_HARDWARE
endif

ifdef LOGGING
override CFLAGS += -DMEMENTOS_LOGGING
endif

VTHRESHDEFAULT=2.5
ifeq ($(VTHRESH),)
 override VTHRESH=$(VTHRESHDEFAULT)
endif

TIMERINTDEFAULT=20000
ifeq ($(TIMERINT),)
 override TIMERINT=$(TIMERINTDEFAULT)
endif

# which flavors to build
FLAVORS=plainclang plainmspgcc latch return timer
TARGETS=$(foreach flavor,$(FLAVORS),$(TARGET)+$(flavor))

all: $(TARGETS)

mementos.c: include/mementos.h
include/mementos.h:
	scripts/setthresh.pl $(VTHRESH) $(TIMERINT) include/mementos.h.tmpl >include/mementos.h

.c.bc: include/mementos.h
	$(CC) $(CFLAGS) -c -o $@ $<

install: all
	for t in $(TARGETS); do \
		install -m 0755 $$t $(LLVMBUILD)/../mspsim/firmware/$$t; \
	done

# plain target built with clang
$(TARGET)+plainclang: $(TARGET).c include/mementos.h
	$(CC) $(CFLAGS)   -o $@.bc -c $<
	$(LLC)            -o $@.s $@.bc
	$(MCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

# plain target built with mspgcc
$(TARGET)+plainmspgcc: $(TARGET).c include/mementos.h
	$(MCC) $(MCFLAGS) -o $@ $< $(MCLIBS)


# standalone Mementos bitcode (for linking against)
mementos+latch.bc: mementos.c
	$(CC) $(CFLAGS) -o $@ -DMEMENTOS_LATCH -c $<
mementos+return.bc: mementos.c
	$(CC) $(CFLAGS) -o $@ -DMEMENTOS_RETURN -c $<
mementos+timer+latch.bc: mementos.c
	$(CC) $(CFLAGS) -o $@ -DMEMENTOS_TIMER -DMEMENTOS_LATCH -c $<

# instrument all loop latches
$(TARGET)+latch: $(TARGET).c include/mementos.h mementos+latch.bc
	$(CC) $(CFLAGS)   -o $@.bc -DMEMENTOS_LATCH -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM)/bin/llvm-link         -o $@+gsize+mementos.bc $@+gsize.bc mementos+latch.bc
	$(OPT_LATCH)      -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(MCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

# instrument all function returns
$(TARGET)+return: $(TARGET).c include/mementos.h mementos+return.bc
	$(CC) $(CFLAGS)   -o $@.bc -DMEMENTOS_RETURN -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM)/bin/llvm-link         -o $@+gsize+mementos.bc $@+gsize.bc mementos+return.bc
	$(OPT_RETURN)     -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(MCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

# instrument loop latches with trigger points, but these trigger points won't
# run at all (i.e., return without doing an energy check) unless the
# 'ok_to_checkpoint' flag is set, which happens every TIMER_INTERVAL
# (include/mementos.h) cycles.
$(TARGET)+timer: $(TARGET).c include/mementos.h mementos+timer+latch.bc
	$(CC) $(CFLAGS)   -o $@.bc -DMEMENTOS_TIMER -DMEMENTOS_LATCH -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM)/bin/llvm-link         -o $@+gsize+mementos.bc $@+gsize.bc mementos+timer+latch.bc
	$(OPT_LATCH)      -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(MCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

logme: logme.c
	msp430-gcc -mmcu=msp430x2132 -o $@ $<

strip: $(TARGETS)
	msp430-strip $(TARGETS)

gdbcommands: $(TARGETS)
	for t in $(TARGETS); do \
		MEMENTOS_INTERESTING='restore|log_event' \
		scripts/gen-gdb-commands.sh $$t > $$t.gdb ; \
	done

clean:
	$(RM) $(TARGETS) *.o samples/*.o *.bc samples/*.bc *.s samples/*.s \
		include/mementos.h logme *.gdb samples/*.gdb
