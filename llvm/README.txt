Building:

1. Build LLVM with CMake.  For the rest of this README we'll assume you've
   installed LLVM at /opt/llvm.

2. Build the Mementos pass with CMake:

    $ cd mementos/llvm
    $ cmake -DCMAKE_PREFIX_PATH=/opt/llvm -DCMAKE_INSTALL_PREFIX=/opt/llvm .
    $ make
    $ make install
