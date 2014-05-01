#
### Mementos build system.
#
# To build against a <program>.c (default: samples/crc-vanilla.c), run:
#  make TARGET=program
# To build with hardware support, define HARDWARE:
#  make HARDWARE=1 [...]
# To build with hardware support *and* support for logging to an outboard
# device (recommended), run:
#  make HARDWARE=1 LOGGING=1 [...]
#

# conditional definitions
TARGET ?= samples/crc-vanilla

include Makefile.common

MYLIBS = msp430builtins.o
MCLIBS += $(MYLIBS)

.DUMMY: all clean distclean gdbcommands install strip

MEMENTOS_SRCS =

ifdef TIMER
override CFLAGS += -DMEMENTOS_VOLTAGE_CHECK
endif

ifdef TIMER
override CFLAGS += -DMEMENTOS_TIMER
endif

ifdef HARDWARE
override CFLAGS += -DMEMENTOS_HARDWARE
MEMENTOS_SRCS += mementos_hw.c
endif

ifdef LOGGING
override CFLAGS += -DMEMENTOS_LOGGING
MEMENTOS_SRCS += mementos_logging.c
endif

ifdef FRAM
override CFLAGS += -DMEMENTOS_FRAM
MEMENTOS_SRCS += mementos_fram.c
else
override CFLAGS += -DMEMENTOS_FLASH
MEMENTOS_SRCS += mementos_flash.c
endif

MEMENTOS_OBJS = $(MEMENTOS_SRCS:.c=.bc)

VTHRESHDEFAULT=2.7
ifeq ($(VTHRESH),)
 override VTHRESH=$(VTHRESHDEFAULT)
endif

TIMERINTDEFAULT=20000
ifeq ($(TIMERINT),)
 override TIMERINT=$(TIMERINTDEFAULT)
endif

# which flavors to build
FLAVORS=plainclang plainmspgcc latch return timer oracle
TARGETS=$(MYLIBS) $(foreach flavor,$(FLAVORS),$(TARGET)+$(flavor))

all: $(TARGETS)

mementos.c: include/mementos.h
include/mementos.h: include/mementos.h.tmpl
	scripts/setthresh.pl $(VTHRESH) $(TIMERINT) include/mementos.h.tmpl >include/mementos.h

.c.bc: include/mementos.h
	$(CLANG) $(CFLAGS) -c -o $@ $<

install: $(TARGETS)
ifeq ($(MSPSIM),)
	@echo "No mspsim directory defined, so nowhere to install."
	@echo "Reconfigure --with-mspsim=/path/to/mspsim to specify" \
		"an installation path for built programs." 1>&2
else
	for t in $(TARGETS); do \
		install -m 0755 $$t $(MSPSIM)/firmware/`basename "$$t"`; \
	done
endif

# plain target built with clang
$(TARGET)+plainclang: $(TARGET).c include/mementos.h $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -c $<
	$(LLC)            -o $@.s $@.bc
	$(MSPGCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

# plain target built with mspgcc
$(TARGET)+plainmspgcc: $(TARGET).c include/mementos.h
	$(MSPGCC) $(MCFLAGS) -o $@ $< $(MCLIBS)

# standalone Mementos bitcode (for linking against)
mementos+latch.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_LATCH -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(MEMENTOS_OBJS)
mementos+return.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_RETURN -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(MEMENTOS_OBJS)
mementos+timer+latch.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_TIMER -DMEMENTOS_LATCH -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(MEMENTOS_OBJS)
mementos+oracle.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_ORACLE -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(MEMENTOS_OBJS)

# instrument all loop latches
$(TARGET)+latch: $(TARGET).c include/mementos.h mementos+latch.bc $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -DMEMENTOS_LATCH -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM_LINK)      -o $@+gsize+mementos.bc $@+gsize.bc mementos+latch.bc
	$(OPT_LATCH)      -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(MSPGCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

# instrument all function returns
$(TARGET)+return: $(TARGET).c include/mementos.h mementos+return.bc $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -DMEMENTOS_RETURN -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM_LINK)      -o $@+gsize+mementos.bc $@+gsize.bc mementos+return.bc
	$(OPT_RETURN)     -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(MSPGCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

# instrument loop latches with trigger points, but these trigger points won't
# run at all (i.e., return without doing an energy check) unless the
# 'ok_to_checkpoint' flag is set, which happens every TIMER_INTERVAL
# (include/mementos.h) cycles.
$(TARGET)+timer: $(TARGET).c include/mementos.h mementos+timer+latch.bc $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -DMEMENTOS_TIMER -DMEMENTOS_LATCH -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM_LINK)      -o $@+gsize+mementos.bc $@+gsize.bc mementos+timer+latch.bc
	$(OPT_LATCH)      -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(MSPGCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

# link against mementos but don't instrument code
$(TARGET)+oracle: $(TARGET).c include/mementos.h mementos+oracle.bc $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -DMEMENTOS_ORACLE -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM_LINK)      -o $@+gsize+mementos.bc $@+gsize.bc mementos+oracle.bc
	$(LLC)            -o $@.s $@+gsize+mementos.bc
	$(MSPGCC) $(MCFLAGS) -o $@ $@.s $(MCLIBS)

msp430builtins.o: msp430builtins.S
	$(MSPGCC) $(MCFLAGS) -c -o $@ $<

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

distclean:
	$(RM) config.log config.status
