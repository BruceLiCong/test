#!/usr/bin/python

import math

def problem7(n):
    ps = []
    i = 1
    j = 1
    if n == 1:
        return 2
    while j < n:
        i = i + 2
        ip = True
        for pi in ps:
            if (pi * pi) > i:
                break
            if (i%pi) == 0:
                ip = False
                break
        if ip:
            ps.append(i)
            j = j + 1
    return i

print problem7(10001)
    
