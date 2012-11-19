#include "common.h"
#ifndef __MSP430__
#include <stdio.h>
#endif

int arr[20] = {8, 19, 10, 7, 9,
               4, 2, 14, 18, 0,
               16, 5, 13, 11, 15,
               17, 3, 1, 12, 6};

// adapted, with a snicker, from
// http://www.haskell.org/haskellwiki/Introduction#Quicksort_in_C
void qsort(int a[], int lo, int hi) {
    int h, l, p, t;

    if (lo < hi) {
        l = lo;
        h = hi;
        p = a[hi];

        do {
            while ((l < h) && (a[l] <= p)) l = l+1;
            while ((h > l) && (a[h] >= p)) h = h-1;
            if (l < h) {
                t = a[l];
                a[l] = a[h];
                a[h] = t;
            }
        } while (l < h);

        a[hi] = a[l];
        a[l] = p;

        qsort(a, lo, l-1);
        qsort(a, l+1, hi);
    }
}

MEMENTOS_MAIN_ATTRIBUTES
int main (void) {
    int i;
    qsort(arr, 0, 19);
#ifndef __MSP430__
    printf("{");
    for (i = 0; i < 20; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("}\n");
#endif
}
