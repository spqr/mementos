#!/usr/bin/env python
import os
import sys
from difflib import unified_diff

def levenshtein(a,b):
    """Calculates the Levenshtein distance between a and b.
       From http://hetland.org/coding/python/levenshtein.py"""
    n, m = len(a), len(b)
    if n > m:
        # Make sure n <= m, to use O(min(n,m)) space
        a,b = b,a
        n,m = m,n
    current = range(n+1)
    for i in range(1,m+1):
        previous, current = current, [i]+[0]*n
        for j in range(1,n+1):
            add, delete = previous[j]+1, current[j-1]+1
            change = previous[j-1]
            if a[j-1] != b[i-1]:
                change = change + 1
            current[j] = min(add, delete, change)
    return current[n]

def sort_by_num_ext (alist):
    def num_ext (filename):
        return int(os.path.splitext(filename)[-1][1:])
    blist = alist[:]
    blist.sort(lambda a,b: cmp(num_ext(a), num_ext(b)))
    return blist

def mean (alist):
    return 1.0 * sum(alist) / len(alist)

def median (alist):
    blist = alist[:]
    alist.sort()
    if (len(alist) % 2) == 0:
        return (alist[len(alist)/2 - 1] + alist[len(alist)/2]) / 2.0
    else:
        return alist[len(alist)/2]

prefix = sys.argv[1]
files = sort_by_num_ext(filter(lambda x: x.startswith(prefix+'.'), \
            os.listdir('.')))
print files

distances = []
for i in range(len(files) - 1):
    fa = files[i]
    fb = files[i+1]
    a = open(fa, 'r').readlines()
    b = open(fb, 'r').readlines()
    for x in unified_diff(a, b, fa, fb):
        sys.stdout.write(x)
    distances.append(levenshtein(a, b))

print "Levenshtein distances (min, mean, median, max): %s, %s, %s, %s" % ( \
        min(distances), mean(distances), median(distances), max(distances))
