# Mementos

A system that makes programs robust against frequent power loss on
intermittently powered computers.

Currently implemented for the MSP430 family of processors.

The [ASPLOS '11 paper](https://spqr.eecs.umich.edu/papers/ransford-mementos-asplos11.pdf) describes Mementos in detail.

See [license.txt](https://github.com/spqr/mementos/raw/master/license.txt) for licensing terms.

## Special Wolverine instructions ##

Use `make -f Makefile.wolverine` to build `libmementos_fram.a`, which you can
link into your own programs.

## Building and Installing #

**Note: Mementos now works on Wolverine (MSP430FRXXXX) hardware.  Check out the
`wolverine` branch!**

0. Build and install [LLVM](http://llvm.org/) and
[Clang](http://clang.llvm.org/) using the [CMake-style
build](http://llvm.org/docs/CMake.html).  The rest of these instructions assume
you have chosen to put your LLVM installation in `/opt/llvm`.

```sh
# download LLVM, Clang, compiler-rt to ~/tmp/llvm-3.5.0.src per LLVM's
# build instructions
...
$ cmake -DLLVM_TARGETS_TO_BUILD="X86;MSP430" -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/opt/llvm ~/tmp/llvm-3.5.0.src
```

1. Install an MSP430 compiler toolchain such as mspgcc or gcc 4.9 (which
includes an MSP430 backend in later releases).  If you are using mspgcc, ensure
that `msp430-gcc` is in your `$PATH`.

2. Clone the Mementos repository.
```sh
$ git clone git@github.com:spqr/mementos.git
$ export MEMENTOS="$PWD/mementos"
```

3. Build and install Mementos's LLVM passes:
```sh
$ cd "$MEMENTOS"/llvm
$ mkdir build && cd build
$ cmake -DLLVM_ROOT=/opt/llvm ..
$ make install
```

4. Configure Mementos's non-LLVM component:
```sh
$ cd "$MEMENTOS"
$ ./configure
```

6. Fetch the Mementos fork of MSPsim:
```sh
$ cd /opt/mementos/src
$ git clone git://github.com/ransford/mspsim.git mspsim
$ cd mspsim
$ ant
```

Now you can [instrument your programs with
Mementos](https://github.com/spqr/mementos/wiki/Mementos-izing-a-Program) and
run them in MSPsim.

### Dynamic symbol errors when running opt

On OS X (and possibly other OSes), if you get an [error about a missing symbol](http://llvm.org/PR13144) when attempting to compiling programs with Mementos, you should edit `/opt/mementos/src/llvm/tools/opt/Makefile` and add the line `KEEP_SYMBOLS := 1`, then `cd /opt/mementos/build/llvm && make install`.

## Mementos-izing a Program #

By _Mementosize_ a program, we mean instrument the program with energy checks called _trigger points_.  By default, Mementos will build several variants of your program around several different instrumentation strategies: loop-latch instrumentation, function-return instrumentation, and timer-aided loop-latch instrumentation.  We'll call these variants `myfoo+latch`, `myfoo+return`, and `myfoo+timer`.  It additionally tries to build separate "plain" (uninstrumented) versions with clang and mspgcc.

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

## Caveats #

Things known not to work:

* If you have your own TimerA interrupt defined, it will clash with the one Mementos tries to introduce in the `myfoo+timer` variant.
