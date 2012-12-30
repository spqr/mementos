CC=msp430-gcc
CFLAGS=-mmcu=msp430f2618
SOURCES=$(wildcard *.c)
EXECUTABLES=$(SOURCES:.c=)

.DUMMY: clean all

all: $(EXECUTABLES)

clean:
	$(RM) $(EXECUTABLES)
