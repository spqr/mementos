make TARGET=samples/sense clean
make TARGET=samples/sense install

make TARGET=samples/crc-vanilla2 clean
make TARGET=samples/crc-vanilla2 install

make TARGET=samples/rsa64 clean
make MCU=msp430x1611 VTHRESH=2.8 TARGET=samples/rsa64 install
