# Mementos

A system that makes programs robust against frequent power loss on
intermittently powered computers.

Currently implemented for the MSP430 family of processors.

The [ASPLOS '11 paper](https://spqr.eecs.umich.edu/papers/ransford-mementos-asplos11.pdf) describes Mementos in detail.

See [license.txt](https://github.com/spqr/mementos/raw/master/license.txt) for licensing terms.

## Building and Installing #

0. Choose an install location like `/opt/mementos` and make a `build` directory.
```sh
$ sudo mkdir -p /opt/mementos
$ sudo chown -R $USER /opt/mementos
$ cd /opt/mementos
$ mkdir -p src build/llvm
```

1. Build and install [LLVM](http://llvm.org/releases/) and [clang](http://clang.llvm.org/).  Use the [CMake-style build](http://llvm.org/docs/CMake.html).
```sh
$ cd /opt/mementos/src
$ wget http://llvm.org/releases/3.3/llvm-3.3.src.tar.gz
$ wget http://llvm.org/releases/3.3/cfe-3.3.src.tar.gz
$ tar zxvf llvm-3.3.src.tar.gz
$ cd llvm-3.3-src/tools
$ tar zxvf ../../cfe-3.3.src.tar.gz
$ mv cfe-3.3-src clang
$ cd /opt/mementos/build/llvm
$ cmake -DCMAKE_INSTALL_PREFIX=/opt/mementos ../../src/llvm-3.3-src
$ make
$ make install
```

3. Install [mspgcc](http://mspgcc.sourceforge.net/).  If you're on a Mac, the easiest way (as of December 2011) is via [Fink](http://www.finkproject.org/) or [MacPorts](http://www.macports.org/).
```sh
$ fink install msp430-gcc msp430-libc mspdebug
# or
$ sudo port install msp430-gcc msp430-libc mspdebug
```
If you're on Ubuntu, use the gcc-msp430 package:
```sh
$ sudo aptitude install gcc-msp430
```

4. Clone the _mementos_ repository and build and install its LLVM component.  Manipulate your `$PATH` as below to build this component with the Clang you just built.
```sh
$ cd /opt/mementos/src
$ git clone git://github.com/spqr/mementos.git mementos
$ cd mementos/llvm
$ cmake -DCMAKE_PREFIX_PATH=/opt/mementos -DCMAKE_INSTALL_PREFIX=/opt/mementos .
$ make
$ make install
```

5. Configure Mementos's non-LLVM component:
```sh
$ cd /opt/mementos/src/mementos
$ ./configure
```

6. Fetch the Mementos fork of MSPsim:
```sh
$ cd /opt/mementos/src
$ git clone git://github.com/ransford/mspsim.git mspsim
$ cd mspsim
$ ant
```

Now you can [instrument your programs with Mementos](https://github.com/spqr/mementos/wiki/Mementos-izing-a-Program) and run them in MSPsim.

### Dynamic symbol errors when running opt

On OS X (and possibly other OSes), if you get an [error about a missing symbol](http://llvm.org/PR13144) when attempting to compiling programs with Mementos, you should edit `/opt/mementos/src/llvm/tools/opt/Makefile` and add the line `KEEP_SYMBOLS := 1`, then `cd /opt/mementos/build/llvm && make install`.

## Mementos-izing a Program #

By _Mementosize_ a program, we mean instrument the program with energy checks called _trigger points_.  By default, Mementos will build several variants of your program around several different instrumentation strategies: loop-latch instrumentation, function-return instrumentation, and timer-aided loop-latch instrumentation.  We'll call these variants `myfoo+latch`, `myfoo+return`, and `myfoo+timer`.  It additionally tries to build separate "plain" (uninstrumented) versions with clang and mspgcc.

XXX We'll write more here soon.

1. Make sure the program is in a single `.c` file; let's call it e.g. `myfoo.c`.
2. Put the program in `src/mementos` (see below).
2. Build it against Mementos:
 <pre>
$ cd /opt/mementos/src/mementos
$ make TARGET=myfoo
</pre>
 If the build succeeds, you now have several executables representing different versions of your program.  These are ready to run against mspsim.

Temporary link to download simulator based on MSPsim:
[[http://www.cs.umass.edu/~ransford/mementos-downloads/mspsim-mementos-r571.tar.gz]]

XXX We'll write more here soon.

## Caveats #

Things known not to work:

* If you have your own TimerA interrupt defined, it will clash with the one Mementos tries to introduce in the `myfoo+timer` variant.
