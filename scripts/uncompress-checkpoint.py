#!/usr/bin/env python
import sys

def popcount (x):
    return sum([int(i) for i in bin(x)[2:]])

filename = sys.argv[1]

lines = open(filename, 'r').readlines()
data = [int(line, 16) for line in lines]
totalsize = len(data) * 2

(stacksize, datasize) = data[0] >> 8, data[0] & 0xff
regsize = popcount(data[1]) * 2

if (regsize + stacksize + datasize + 2 + 2) != totalsize:
    raise Exception("size mismatch! corrupt checkpoint bundle.")

print '%s %s' % (totalsize, (stacksize + datasize + 2 + 30))

