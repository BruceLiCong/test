#!/usr/bin/python

import math
    
def gcd(a, b):
    c = a%b
    if c == 0:
        return b
    return gcd(max(b,c), min(b,c))

def lcm(a, b):
    return (a * b / (gcd(a, b)))
    
def problem5(n):
    i = 1
    s = 1
    while i <= n:
        s = lcm(max(s,i), min(s,i))
        i = i + 1
    return s


print problem5(20)
    
