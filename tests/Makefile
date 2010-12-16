CC=msp430-gcc
CFLAGS=-mmcu=msp430x2131
SOURCES=$(wildcard *.c)
EXECUTABLES=$(SOURCES:.c=)

.DUMMY: clean all

all: $(EXECUTABLES)

clean:
	$(RM) $(EXECUTABLES)
