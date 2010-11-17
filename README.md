# Prerequisites #

1. Build and install [LLVM](http://llvm.org/releases/) and [clang](http://clang.llvm.org/).  Don't discard your source tree or (if separate) your build tree.  Make sure the `clang` executable is somewhere in your path.

2. Build and install [mspgcc](http://mspgcc.sourceforge.net/).  Make sure the `msp430-gcc` and `msp430-as` programs are in your path.

3. Clone the _mementos_ repository:
 <pre>
$ git clone git://github.com/ransford/mementos.git
$ cd mementos
</pre>

4. Build Mementos's LLVM passes:
 <pre>
$ cd llvm
$ ./configure
$ make
</pre>


# Mementosizing a Program #

1. Make sure the program is in a single `.c` file; let's call it e.g. `myfoo.c`.
2. Build it against Mementos:
 <pre>
$ make TARGET=myfoo
</pre>

