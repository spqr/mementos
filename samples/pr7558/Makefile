PROGRAM=foo

all: $(PROGRAM).o

$(PROGRAM).ll: $(PROGRAM).c
	clang -O0 -emit-llvm -ccc-host-triple msp430-elf -o $@ -S $<

$(PROGRAM).o: $(PROGRAM).ll
	llc -march=msp430 -combiner-alias-analysis -o $@ $<
