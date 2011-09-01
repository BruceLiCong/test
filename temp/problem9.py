#!/usr/bin/python

def problem9(n):
    a = 1
    b = 1
    c = 1
    maxA = n / 3
    while a < maxA:
        d = n * (n - (a << 1))
        e = (n - a) << 1
        if (d%e) == 0:
            b = d /e
            c = n - a - b
            break
        a = a + 1
    return a * b * c


print problem9(1000)
