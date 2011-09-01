#!/usr/bin/python

def problem10(n):
    ps = []
    i = 3
    s = 2
    if n <= 3:
        return 2
    while i < n:
        ip = True
        for pi in ps:
            if (pi * pi) > i:
                break
            if (i%pi) == 0:
                ip = False
                break
        if ip:
            s = s + i
            ps.append(i)
        i = i + 2
    return s

print problem10(2000000)
