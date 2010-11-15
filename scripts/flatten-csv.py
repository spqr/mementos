#!/usr/bin/python
import sys

filename = sys.argv[1];

prevtime = 0.0
addthismuch = 0.0
chk = False
for line in open(filename, 'r'):
    (time, voltage) = line.rstrip().split(',')
    voltage = float(voltage)
    if time.startswith('<chk>'):
        chk = True
        t = float(time[5:])
    else:
        chk = False
        t = float(time)
    if t < prevtime:
        addthismuch += prevtime
    print "%s%f,%f" % (chk and '<chk>' or '', t+addthismuch, voltage)
    prevtime = t
