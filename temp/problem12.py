#!/usr/bin/python

import math


def problem12(n):
    a = 1
    flag = True
    while flag:
        b = (a * (a + 1)) >> 1
        c = 1
        f = 0
        while (c * c) < b:
            if (b % c) == 0:
                f = f + 2
            c = c + 1
        if (c * c) == b:
            f = f + 1
        if f > n:
            break
        #print a
        a = a + 1
        
    return b



print problem12(500)
